# MQ2LinkDB

This plugin allows you to access links for (hopefully) every item. It stores links in a database and you may call them up manually or via hotkeys, macros or plugins.

## Getting Started

1. Download [items.txt][itemstxt] - [here's][itemstxt] a direct download. (This file is updated daily, you'll want to download it again after new expansions.)
2. Place items.txt in your MQ2 root folder (AKA the "Release" folder, the one that includes all the plugins).
3. In-game, load the plugin with `/plugin mq2linkdb load`
4. Import items.txt with `/link /import`

### Information

Item links are displayed as tells from Linkdb in normal chat windows. This is so you can use the links given.

Incoming chat is scanned for links if specified, and the database is checked for this item. If it's not in database, it will be added. 

### Commands

`/link` - Display statistics  
`/link /import` - Import items.txt  
`/link /max #` - Set maximum number of results (default 10)  
`/link search` - Find items containing search string  
`/link /scan [on|off]` - Turn on and off scanning incoming chat  
`/link /click [on|off]` - Click on the link generated  
`/link /query <query>` - Query the raw items table. i.e.: /link /query tradeskills=1 AND lore LIKE '%lbs.%' LIMIT 25

### TLOs

`string` `${LinkDB[name]}` - Display the link found by name. Use =name for an exact match.

The TLO LinkDB is added by this plugin. The LinkDB TLO supports a simple lookup for items by name and returns the item's link text.

The TLO supports substring matches and exact matches. If you pass =Item Name to the TLO, it will do an exact match. If you just do Item Name, then it will use a prefix match.
If there are multiple items (i.e. multiple items with the same name in an exact, or multiple items with the same prefix in a non-exact), then the TLO will return the first
match and you will have no idea there were multiple results.

Examples:

Get several links to items that include the word "Wurmslayer."

```txt
/link Wurmslayer
```

If the item is not found, the TLO returns an empty string, so you probably don't want to be directly shouting about Baby Joseph Sayer in your backpack. If you do and misspell his name, you will end up shouting about an empty string which isn't recommended.

```txt
/shout OMG I'm a dork! I have ${LinkDB[=Baby Joseph Sayer]} in my pack. Ha!
```

## Authors

* **Ziggy** - *Initial work*
* **SwiftyMUSE** - Updates/DB Maintenance
* See [Changelog](CHANGELOG.md) for pre-git changes.


[itemstxt]: https://www.redguides.com/community/resources/items-txt-used-for-mq2linkdb.1720/download "Items.txt"
