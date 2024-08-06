Input:
    Console interface with commands.
    
Commands:
    Command Name                Behaviour                           Aliases
    add "task"                  Adds task with text "task"          a, new

    add "task" date             Adds task with text "task" and      a, new
                                a completion date of date
    
    add "task" date time        Adds task with text "task" and      a, new
                                a completion date of date and
                                time

    rm index                    Removes task at index               remove, delete, del, r

    rm "tagname"                Removes all tasks tagged with
                                "tagname"

    complete index              Marks a task complete at            x, mark
                                specified index

    complete "tagname"          Marks all tasks with a              x, mark
                                specified tag 

    reset index                 Unmarks a task at index             rst, undo, u

    reset "tagname"             Unmarks a task with tag             rst, undo, u

    edit index "task"           Sets the task at index to have      e
                                the text "task"

    view num                    Switches to num page                v 

    previous                    Goes back a page                    p, prev

    next                        Goes forward a page                 n, next

    createtag "tagname"         Creates a new tag with name         createtag, tc   
                                "tagname"

    tag index "tagname"         Gives task at index the tag         addtag, ta, t
                                specified, if tag does not 
                                exist it is created

    untag index                 Removes all tags from task          None
                                at index

    filter "tagname"            Filters by a tag                    f

    sort sortfilter             Sorts by a specified filter         s  
    
    datetask index date         Gives a task specified date         dt, date
    
Process

    Read input from command line
    Split the command from spaces
    Parse the command

    TaskList - stores all tasks
    ShowingTaskList - stores filtered tasks

    
    