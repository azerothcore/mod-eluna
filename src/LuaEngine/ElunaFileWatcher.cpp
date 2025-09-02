/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "ElunaFileWatcher.h"
#include "LuaEngine.h"
#include "ElunaUtility.h"
#include <boost/filesystem.hpp>

ElunaFileWatcher::ElunaFileWatcher() : running(false), checkInterval(1)
{
}

ElunaFileWatcher::~ElunaFileWatcher()
{
    StopWatching();
}

void ElunaFileWatcher::StartWatching(const std::string& scriptPath, uint32 intervalSeconds)
{
    if (running.load())
    {
        ELUNA_LOG_DEBUG("[ElunaFileWatcher]: Already watching files");
        return;
    }

    if (scriptPath.empty())
    {
        ELUNA_LOG_ERROR("[ElunaFileWatcher]: Cannot start watching - script path is empty");
        return;
    }

    watchPath = scriptPath;
    checkInterval = intervalSeconds;
    running.store(true);

    ScanDirectory(watchPath);

    watcherThread = std::thread(&ElunaFileWatcher::WatchLoop, this);
    
    ELUNA_LOG_INFO("[ElunaFileWatcher]: Started watching '{}' (interval: {}s)", watchPath, checkInterval);
}

void ElunaFileWatcher::StopWatching()
{
    if (!running.load())
        return;

    running.store(false);

    if (watcherThread.joinable())
        watcherThread.join();

    fileTimestamps.clear();
    
    ELUNA_LOG_INFO("[ElunaFileWatcher]: Stopped watching files");
}

void ElunaFileWatcher::WatchLoop()
{
    while (running.load())
    {
        try
        {
            CheckForChanges();
        }
        catch (const std::exception& e)
        {
            ELUNA_LOG_ERROR("[ElunaFileWatcher]: Error during file watching: {}", e.what());
        }

        std::this_thread::sleep_for(std::chrono::seconds(checkInterval));
    }
}

bool ElunaFileWatcher::IsWatchedFileType(const std::string& filename) {
    return (filename.length() >= 4 && filename.substr(filename.length() - 4) == ".lua") ||
        (filename.length() >= 4 && filename.substr(filename.length() - 4) == ".ext") ||
        (filename.length() >= 5 && filename.substr(filename.length() - 5) == ".moon");
}

void ElunaFileWatcher::ScanDirectory(const std::string& path)
{
    try
    {
        boost::filesystem::path dir(path);
        
        if (!boost::filesystem::exists(dir) || !boost::filesystem::is_directory(dir))
            return;

        boost::filesystem::directory_iterator end_iter;
        
        for (boost::filesystem::directory_iterator dir_iter(dir); dir_iter != end_iter; ++dir_iter)
        {
            std::string fullpath = dir_iter->path().generic_string();
            
            if (boost::filesystem::is_directory(dir_iter->status()))
            {
                ScanDirectory(fullpath);
            }
            else if (boost::filesystem::is_regular_file(dir_iter->status()))
            {
                std::string filename = dir_iter->path().filename().generic_string();
                
                if (IsWatchedFileType(filename))
                {
                    fileTimestamps[fullpath] = boost::filesystem::last_write_time(dir_iter->path());
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        ELUNA_LOG_ERROR("[ElunaFileWatcher]: Error scanning directory '{}': {}", path, e.what());
    }
}

void ElunaFileWatcher::CheckForChanges()
{
    bool hasChanges = false;
    
    try
    {
        boost::filesystem::path dir(watchPath);
        
        if (!boost::filesystem::exists(dir) || !boost::filesystem::is_directory(dir))
            return;

        boost::filesystem::directory_iterator end_iter;
        
        for (boost::filesystem::directory_iterator dir_iter(dir); dir_iter != end_iter; ++dir_iter)
        {
            if (ShouldReloadFile(dir_iter->path().generic_string()))
                hasChanges = true;
        }
        
        for (auto it = fileTimestamps.begin(); it != fileTimestamps.end();)
        {
            if (!boost::filesystem::exists(it->first))
            {
                ELUNA_LOG_DEBUG("[ElunaFileWatcher]: File deleted: {}", it->first);
                it = fileTimestamps.erase(it);
                hasChanges = true;
            }
            else
            {
                ++it;
            }
        }
    }
    catch (const std::exception& e)
    {
        ELUNA_LOG_ERROR("[ElunaFileWatcher]: Error checking for changes: {}", e.what());
        return;
    }

    if (hasChanges)
    {
        ELUNA_LOG_INFO("[ElunaFileWatcher]: Lua script changes detected - triggering reload");
        Eluna::ReloadEluna();
        
        ScanDirectory(watchPath);
    }
}

bool ElunaFileWatcher::ShouldReloadFile(const std::string& filepath)
{
    try
    {
        boost::filesystem::path file(filepath);
        
        if (boost::filesystem::is_directory(file))
        {
            boost::filesystem::directory_iterator end_iter;
            
            for (boost::filesystem::directory_iterator dir_iter(file); dir_iter != end_iter; ++dir_iter)
            {
                if (ShouldReloadFile(dir_iter->path().generic_string()))
                    return true;
            }
            return false;
        }
        
        if (!boost::filesystem::is_regular_file(file))
            return false;
            
        std::string filename = file.filename().generic_string();

        if (!IsWatchedFileType(filename)) return false;
            
        auto currentTime = boost::filesystem::last_write_time(file);
        auto it = fileTimestamps.find(filepath);
        
        if (it == fileTimestamps.end())
        {
            ELUNA_LOG_DEBUG("[ElunaFileWatcher]: New file detected: {}", filepath);
            fileTimestamps[filepath] = currentTime;
            return true;
        }
        
        if (it->second != currentTime)
        {
            ELUNA_LOG_DEBUG("[ElunaFileWatcher]: File modified: {}", filepath);
            it->second = currentTime;
            return true;
        }
    }
    catch (const std::exception& e)
    {
        ELUNA_LOG_ERROR("[ElunaFileWatcher]: Error checking file '{}': {}", filepath, e.what());
    }
    
    return false;
}
