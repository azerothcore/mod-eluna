/*
* Copyright (C) 2010 - 2025 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef LOOTMETHODS_H
#define LOOTMETHODS_H

/***
 * Inherits all methods from: none
 */
namespace LuaLoot
{
    int IsLooted(lua_State* L, Loot* loot)
    {
        Eluna::Push(L, loot->isLooted());
        return 1;
    }

    int AddItem(lua_State* L, Loot* loot)
    {
        uint32 itemid = Eluna::CHECKVAL<uint32>(L, 2);
        uint8 min_count = Eluna::CHECKVAL<uint8>(L, 3);
        uint8 max_count = Eluna::CHECKVAL<uint8>(L, 4);
        float chance = Eluna::CHECKVAL<float>(L, 5);
        uint16 loot_mode = Eluna::CHECKVAL<uint16>(L, 6);
        bool needs_quest = Eluna::CHECKVAL<bool>(L, 7, false);

        for (LootItem &lootitem : loot->items)
        {
            if (lootitem.itemid == itemid && lootitem.count < 255)
            {
                lootitem.count += min_count;
                return 0;
            }
        }

        LootStoreItem newLootStoreItem(itemid, 0, chance, needs_quest, loot_mode, 0, min_count, max_count);
        loot->AddItem(newLootStoreItem);

        return 0;
    }

    int HasItem(lua_State* L, Loot* loot)
    {
        uint32 itemid = Eluna::CHECKVAL<uint32>(L, 2, false);
        uint32 count = Eluna::CHECKVAL<uint32>(L, 3, false);
        bool has_item = false;

        if (itemid)
        {
            for (const LootItem &lootitem : loot->items)
            {
                if (lootitem.itemid == itemid && (count == 0 || lootitem.count == count))
                {
                    has_item = true;
                    break;
                }
            }
        }
        else
        {
            for (const LootItem &lootitem : loot->items)
            {
                if (lootitem.itemid && lootitem.itemid != 0)
                {
                    has_item = true;
                    break;
                }
            }
        }

        Eluna::Push(L, has_item);
        return 1;
    }

    int RemoveItem(lua_State* L, Loot* loot)
    {
        uint32 itemid = Eluna::CHECKVAL<uint32>(L, 2);
        bool isCountSpecified = Eluna::CHECKVAL<uint32>(L, 3, false);
        uint32 count = isCountSpecified ? Eluna::CHECKVAL<uint32>(L, 4) : 0;

        for (auto it = loot->items.begin(); it != loot->items.end();)
        {
            if (it->itemid == itemid)
            {
                if (isCountSpecified)
                {
                    if (it->count > count)
                    {
                        it->count -= count;
                        break;
                    }
                    else
                    {
                        count -= it->count;
                    }
                }
                it = loot->items.erase(it);
            }
            else
            {
                ++it;
            }
        }
        return 0;
    }

    int GetMoney(lua_State* L, Loot* loot)
    {
        Eluna::Push(L, loot->gold);
        return 1;
    }

    int SetMoney(lua_State* L, Loot* loot)
    {
        uint32 gold = Eluna::CHECKVAL<uint32>(L, 2);

        loot->gold = gold;
        return 0;
    }

    int GenerateMoney(lua_State* L, Loot* loot)
    {
        uint32 min_gold = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 max_gold = Eluna::CHECKVAL<uint32>(L, 3);

        loot->generateMoneyLoot(min_gold, max_gold);
        return 0;
    }

    int Clear(lua_State* /*L*/, Loot* loot)
    {
        loot->clear();
        return 0;
    }

    int SetUnlootedCount(lua_State* L, Loot* loot)
    {
        uint32 count = Eluna::CHECKVAL<uint32>(L, 2);

        loot->unlootedCount = count;
        return 0;
    }

    int GetUnlootedCount(lua_State* L, Loot* loot)
    {
        Eluna::Push(L, loot->unlootedCount);
        return 1;
    }

    int GetItems(lua_State* L, Loot* loot)
    {
        lua_createtable(L, loot->items.size(), 0);
        int tbl = lua_gettop(L);

        for (unsigned int i = 0; i < loot->items.size(); i++)
        {
            lua_newtable(L);

            Eluna::Push(L, loot->items[i].itemid);
            lua_setfield(L, -2, "id");

            Eluna::Push(L, loot->items[i].itemIndex);
            lua_setfield(L, -2, "index");

            Eluna::Push(L, loot->items[i].count);
            lua_setfield(L, -2, "count");

            Eluna::Push(L, loot->items[i].needs_quest);
            lua_setfield(L, -2, "needs_quest");

            Eluna::Push(L, loot->items[i].is_looted);
            lua_setfield(L, -2, "is_looted");

            lua_rawseti(L, tbl, i + 1);
        }

        lua_settop(L, tbl);
        return 1;
    }

    int UpdateItemIndex(lua_State* /*L*/, Loot* loot)
    {
        for (unsigned int i = 0; i < loot->items.size(); i++)
            loot->items[i].itemIndex = i;

        return 0;
    }

    int SetItemLooted(lua_State* L, Loot* loot)
    {
        uint32 itemid = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 count = Eluna::CHECKVAL<uint32>(L, 3);
        bool looted = Eluna::CHECKVAL<uint32>(L, 4, true);

        for (auto &lootItem : loot->items)
        {
            if (lootItem.itemid == itemid && (count == 0 || lootItem.count == count))
            {
                lootItem.is_looted = looted;
                break;
            }
        }
        return 0;
    }
};
#endif // LOOTMETHODS_H
