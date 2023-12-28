# Change Log

* Beyond - See Git
* 3.4 - Eqmule Jan 09 2020 - Fixed a crash when spell links was parsed.
* 3.3 - Eqmule Jan 08 2020 - Updated to not require an item on cursor to do /link /import and to not poof the item used as the template.
* 3.2  Updated for new link format for TBL.
* 3.1  Updated for new link format on BETA, 11/9/18.
* 3.0 - Eqmule 07-22-2016 - Added string safety.
* 2.4  Updated to include new sodeq db dump format as the input based on old 13th-floor database.
* 2.3  Fixed exact search link clicking. Replaced entire import function to use
       a dump from Lucy. 13th-floor is no longer updating their file so it is of no real use to
       us anymore. I will keep an updated MQ2LinkDB.txt file on the MQ2 site for everyones use.
       The file will be updated on a monthly basis, at best.
* 2.2  Updated for CotF release. Linkbot ability is not working
* 2.1  Updated to fix charm links.  Added all the new fields as defined on 13-floor and corrected
       a long standing issue with an escaped delimiter in the names of 3 items.  You NO LONGER
       have to remove the left and right hand entries, they are created as links correctly.
* 1.7  Added simple TLO for accessing links from item names.
* 1.6  Updated for Broken Seas item format changes. Thanks to ksmith and Nilwean at EQItems. See http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=229
* 1.5  Updated for 12/5 item format changes. Thanks to Nilwean and ksmith at EQItems. See http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=215
* 1.4  Updated for SS expansion. Thanks to ksmith at EQItems. See http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=202
* 1.3  Updated for PoR expansion. Thanks to ksmith at EQItems. See http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=170
* 1.2  Added ScanChat ini setting to toggle whether to snarf links from seen chatlines. Defaults
       to on to simulate current behavior. Also updated for EQItems fixes to their export which
       was missing a field.
* 1.1  Updated to reflect new link format. Thanks to ksmith and Nilwean and topside from http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=145 
* 1.06 Alpha sorts the list
       Makes sure that if an exact match is found, then it's displayed, even if we've already
       displayed max results. Only searches the name for the item name, previously searched the
       whole link (eg: EB41 would have matched 2 items from their hash key)
* 1.05 Fixed the really stupid mistake with item bitfield. And found out a new equation:
       Moved items.txt import into plugin (You can still use it external if you want)
* 1.04 Added Max item list so we don't get too spammed by results.
* 1.03 Added some more clues for debugging purposes (do: /link /quiet to show)
* 1.02 Fixed file locking fudge up. Should now add items to database properly.
* 1.01 Added item index so we know already which items the DB has to save a
       bunch of time with checking when adding new items

Changelog from cpp to be merged above:
// 4.0  Rewritten for mqnext using new item link parsing apis provided by EQLib and
//      string_view. Now is able to parse multiple links on the same message. Parts of
//      defunct linkbot removed.
// 3.4  Fixed a crash when spell links was parsed (Eqmule Jan 09 2020)
// 3.3  Updated to not require an item on cursor to do / link / import and to not poof
//      the item used as the template. (Eqmule Jan 08 2020)
// 3.2  Updated for new link format for TBL.
// 3.1  Updated for new link format for TBL.
// 3.0  Added string safety. (Eqmule 07-22-2016)
// 2.4  Updated to include new sodeq db dump format as the input based on old
//      13th-floor database.
// 2.3  Fixed exact search link clicking. Replaced entire import function to use
//      a dump from Lucy. 13th-floor is no longer updating their file so it is of no
//      real use to us anymore. I will keep an updated MQ2LinkDB.txt file on the MQ2
//      site for everyones use. The file will be updated on a monthly basis, at best.
// 2.2  Updated for CotF release. Linkbot ability is not working
// 2.1  Updated to fix charm links.  Added all the new fields as defined on 13-floor and
//      corrected a long standing issue with an escaped delimiter in the names of 3 items.
//      You NO LONGER have to remove the left and right hand entries, they are created
//      as links correctly.
// 2.0  This version, with linkbot ability, must remain in the VIP forums.
//      Added linkbot functionality using an authorization list.  Will automatically
//      click an exact match link.  Added the ability to retrieve a link based on
//      the item id using /item #.
//
//      Linkbot called with tells using the !link command.  It will respond to the
//      caller with the list of links as if you entered a /link command directly.
// 1.7  Added simple TLO for accessing links from item names.
// 1.6  Updated for Broken Seas item format changes. Thanks to ksmith and
//      Nilwean at EQItems. See
//      http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=229
// 1.5  Updated for 12/5 item format changes. Thanks to Nilwean and ksmith
//      at EQItems. See
//      http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=215
// 1.4  Updated for SS expansion. Thanks to ksmith at EQItems. See
//          http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=202
// 1.3  Updated for PoR expansion. Thanks to ksmith at EQItems. See
//          http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=170
//
// 1.2  Added ScanChat ini setting to toggle whether to snarf links from
//          seen chatlines. Defaults to on to simulate current behavior.
//          Also updated for EQItems fixes to their export which was missing
//          a field.
//
// 1.1  Updated to reflect new link format. Thanks to ksmith and Nilwean
//          and topside from
//          http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=145
//
// 1.06 Alpha sorts the list
//      Makes sure that if an exact match is found, then it's displayed, even
//      if we've already displayed max results
//      Only searches the name for the item name, previously searched the
//      whole link (eg: EB41 would have matched 2 items from their hash key)
//
// 1.05 Fixed the really stupid mistake with item bitfield. And found out
//      a new equation:
//      Moved items.txt import into plugin (You can still use it external if
//      you want)
//
// 1.04 Added Max item list so we don't get too spammed by results.
//
// 1.03 Added some more clues for debugging purposes (do: /link /quiet to show)
//
// 1.02 Fixed file locking fudge up. Should now add items to database properly.
//
// 1.01 Added item index so we know already which items the DB has to save a
//      bunch of time with checking when adding new items
//
// TODO:
// Auto update from 13th floor website
//
