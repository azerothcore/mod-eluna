/*
* Copyright (C) 2010 - 2025 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef LOOTMETHODS_H
#define LOOTMETHODS_H

/***
 * Represents loot that can be obtained from various sources like creatures, gameobjects, or items.
 * 
 * Contains information about items that can be looted, their quantities, money, and loot state.
 *
 * Inherits all methods from: none
 */
namespace LuaLoot
{
    /**
     * Returns `true` if all loot has been taken from this [Loot], returns `false` otherwise.
     *
     * @return bool isLooted
     */
    int IsLooted(lua_State* L, Loot* loot)
    {
        Eluna::Push(L, loot->isLooted());
        return 1;
    }

    /**
     * Adds an item to the [Loot] with the specified parameters.
     *
     * If an item with the same ID already exists and its count is less than 255, the count will be increased instead of adding a new entry.
     *
     * @param uint32 itemId : the ID of the item to add
     * @param uint8 minCount : minimum count of the item
     * @param uint8 maxCount : maximum count of the item
     * @param float chance : chance for the item to drop (0-100)
     * @param uint16 lootMode : loot mode for the item
     * @param bool needsQuest = false : if `true`, the item requires a quest to be looted
     */
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

    /**
     * Returns `true` if the [Loot] contains the specified item, and returns `false` otherwise.
     *
     * @param uint32 itemId = 0 : the ID of the item to check for. If 0, checks if any item exists
     * @param uint32 count = 0 : specific count to check for. If 0, ignores count
     * @return bool hasItem
     */
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

    /**
     * Removes the specified item from the [Loot].
     *
     * If count is specified, removes only that amount. Otherwise removes all items with the ID.
     *
     * @param uint32 itemId : the ID of the item to remove
     * @param bool isCountSpecified = false : if `true`, only removes the specified count
     * @param uint32 count = 0 : amount to remove when isCountSpecified is true
     */
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

    /**
     * Returns the amount of money in this [Loot].
     *
     * @return uint32 money : the amount of money in copper
     */
    int GetMoney(lua_State* L, Loot* loot)
    {
        Eluna::Push(L, loot->gold);
        return 1;
    }

    /**
     * Sets the amount of money in this [Loot].
     *
     * @param uint32 money : the amount of money to set in copper
     */
    int SetMoney(lua_State* L, Loot* loot)
    {
        uint32 gold = Eluna::CHECKVAL<uint32>(L, 2);

        loot->gold = gold;
        return 0;
    }

    /**
     * Generates a random amount of money for this [Loot] within the specified range.
     *
     * @param uint32 minGold : minimum amount of money in copper
     * @param uint32 maxGold : maximum amount of money in copper
     */
    int GenerateMoney(lua_State* L, Loot* loot)
    {
        uint32 min_gold = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 max_gold = Eluna::CHECKVAL<uint32>(L, 3);

        loot->generateMoneyLoot(min_gold, max_gold);
        return 0;
    }

    /**
     * Clears all items and money from this [Loot].
     */
    int Clear(lua_State* /*L*/, Loot* loot)
    {
        loot->clear();
        return 0;
    }

    /**
     * Sets the number of unlooted items in this [Loot].
     *
     * @param uint32 count : the number of unlooted items
     */
    int SetUnlootedCount(lua_State* L, Loot* loot)
    {
        uint32 count = Eluna::CHECKVAL<uint32>(L, 2);

        loot->unlootedCount = count;
        return 0;
    }

    /**
     * Returns the number of unlooted items in this [Loot].
     *
     * @return uint32 unlootedCount
     */
    int GetUnlootedCount(lua_State* L, Loot* loot)
    {
        Eluna::Push(L, loot->unlootedCount);
        return 1;
    }

    /**
     * Returns a table containing all items in this [Loot].
     *
     * Each item is represented as a table with the following fields:
     *   - id: item ID
     *   - index: item index in the loot list
     *   - count: quantity of the item
     *   - needs_quest: whether the item requires a quest
     *   - is_looted: whether the item has been looted
     *
     * @return table items : array of item tables
     */
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

    /**
     * Updates the index of all items in this [Loot] to match their position in the list.
     *
     * This should be called after removing items to ensure indices are sequential.
     */
    int UpdateItemIndex(lua_State* /*L*/, Loot* loot)
    {
        for (unsigned int i = 0; i < loot->items.size(); i++)
            loot->items[i].itemIndex = i;

        return 0;
    }

    /**
     * Sets the looted status of a specific item in this [Loot].
     *
     * @param uint32 itemId : the ID of the item
     * @param uint32 count : specific count to match. If 0, ignores count
     * @param bool looted = true : `true` to mark as looted, `false` to mark as unlooted
     */
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
