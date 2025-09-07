---
tags:
  - tlo
---
# `LinkDB`

<!--tlo-desc-start-->
Contains a form for searching and displaying links, and a data type
<!--tlo-desc-end-->

## Forms
<!--tlo-forms-start-->
### {{ renderMember(type='linkdb', name='LinkDB', params='ѕtring') }}

:   Display the link found by name. Use =name for an exact match.

<!--tlo-forms-end-->

## Associated DataTypes
<!--tlo-datatypes-start-->
## [`linkdb`](datatype-linkdb.md)
{% include-markdown "projects/mq2linkdb/datatype-linkdb.md" start="<!--dt-desc-start-->" end="<!--dt-desc-end-->" trailing-newlines=false %} {{ readMore('projects/mq2linkdb/datatype-linkdb.md') }}
:    <h3>Members</h3>
    {% include-markdown "projects/mq2linkdb/datatype-linkdb.md" start="<!--dt-members-start-->" end="<!--dt-members-end-->" %}
    {% include-markdown "projects/mq2linkdb/datatype-linkdb.md" start="<!--dt-linkrefs-start-->" end="<!--dt-linkrefs-end-->" %}
    <!--tlo-datatypes-end-->

## Examples
<!--tlo-examples-start-->
`/shout OMG I'm a dork! I have ${LinkDB[=Baby Joseph Sayer]} in my pack. Ha!`

`/declare BABYLINK string outer
/varset BABYLINK ${LinkDB[=Baby Joseph Sayer]}`
<!--tlo-examples-end-->

<!--tlo-linkrefs-start-->
[linkdb]: datatype-linkdb.md
<!--tlo-linkrefs-end-->