---
tags:
  - plugin
resource_link: "https://www.redguides.com/community/resources/mq2linkdb.139/"
support_link: "https://www.redguides.com/community/threads/mq2linkdb.66844/"
repository: "https://github.com/RedGuides/MQ2LinkDB"
authors: "brainiac, Ziggy, rswiders, eqmule, Nilwean, ksmith, Derple, Knightly"
tagline: "This plugin gives you a link for (potentially) every item, whether you have it or not."
---

# MQ2LinkDB

<!--desc-start-->
Access links for (hopefully) every item. It stores links in a database and you may call them up manually or via hotkeys, macros or plugins.
<!--desc-end-->

## Commands

<a href="cmd-linkdb/">
{% 
  include-markdown "projects/mq2linkdb/cmd-linkdb.md" 
  start="<!--cmd-syntax-start-->" 
  end="<!--cmd-syntax-end-->" 
%}
</a>
:    {% include-markdown "projects/mq2linkdb/cmd-linkdb.md" 
        start="<!--cmd-desc-start-->" 
        end="<!--cmd-desc-end-->" 
        trailing-newlines=false 
     %} {{ readMore('projects/mq2linkdb/cmd-linkdb.md') }}

## Settings

Item links are displayed as tells from Linkdb in normal chat windows. This is so you can use the links given.

Incoming chat is scanned for links if specified, and the database is checked for this item. If it's not in database, it will be added.

## Quick Start

1. Load the plugin:
   ```bash
   /plugin linkdb load
   ```

2. Import the items database (note the `/` before `import`)
   ```bash
   /linkdb /import
   ```

3. (Optional) To get a link for an item:
   ```bash
   /linkdb "item name"
   ```
  
## Top-Level Objects

## [LinkDB](tlo-linkdb.md)
{% include-markdown "projects/mq2linkdb/tlo-linkdb.md" start="<!--tlo-desc-start-->" end="<!--tlo-desc-end-->" trailing-newlines=false %} {{ readMore('projects/mq2linkdb/tlo-linkdb.md') }}

<h2>Forms</h2>
{% include-markdown "projects/mq2linkdb/tlo-linkdb.md" start="<!--tlo-forms-start-->" end="<!--tlo-forms-end-->" %}
{% include-markdown "projects/mq2linkdb/tlo-linkdb.md" start="<!--tlo-linkrefs-start-->" end="<!--tlo-linkrefs-end-->" %}

## DataTypes

## [linkdb](datatype-linkdb.md)
{% include-markdown "projects/mq2linkdb/datatype-linkdb.md" start="<!--dt-desc-start-->" end="<!--dt-desc-end-->" trailing-newlines=false %} {{ readMore('projects/mq2linkdb/datatype-linkdb.md') }}

<h2>Members</h2>
{% include-markdown "projects/mq2linkdb/datatype-linkdb.md" start="<!--dt-members-start-->" end="<!--dt-members-end-->" %}
{% include-markdown "projects/mq2linkdb/datatype-linkdb.md" start="<!--dt-linkrefs-start-->" end="<!--dt-linkrefs-end-->" %}
