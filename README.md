# Beets
*A personal project to learn C.*

Beets is a todo list console based app.

The goal of the project is to learn C and to create a lightweight console app to keep track of tasks and deadlines.

## Commands
|Command Name| Command Description | Aliases |
| ---------- | ------------------- | ------- |
| add "task" | Adds task with the specified text | a, new |
| add "task" date | Adds task with the specifed text and date. Date can be descriped as year/month/day/hour/minute, in that order. | a, new |
| rm index | Removes the task at the specified index | remove,delete, del, r |
| rm "tagname" - NOT IMPLEMENTED | Removes all tasks tagged with "tagname". | remove, delete, del, r
| complete index | Marks a task at index complete | x, mark |
| complete "tagname" - NOT IMPLEMENTED | Marks all tasks tagged with "tagname" as complete | x, mark |
| reset index | Marks a task at index uncomplete | rst, undo, u |
| reset "tagname" - NOT IMPLEMENTED | Marks all tasks tagged with "tagname" as uncomplete | rst, undo, u |
| edit index "task" | Edits the task at the index to have the new text | e |
| view num | Switch to specified num page | v |
| previous | Goes back a page | p, prev |
| next | Goes to the next page | n |
| createtag "tagname" | Creates a tag with the specifed name "tagname" | tc |
| tag index "tagname" | Gives the task at the index the specified tag "tagname". | addtag, ta, t |
| untag index | Removles all tags from task | None |
| filter "tagname" | Only shows tasks with specified tag "tagname" | f |
| sort sortfilter - NOT IMPLEMENTED | Sorts by a specified filter | s |
| datetask index date | Gives task at index the specified date | date, dt | 
