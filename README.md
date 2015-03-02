# Overview

This directory contains the component of PEOS - the Process Enactment
Operating System - which is an enactment system for the PML process
modeling language.  The source code is the result of a number of
student projects carried out at the University of Colorado, Denver,
and Santa Clara University in the USA, and research projects at Lero,
the Irish Software Engineering Research Centre.  The code is released
under the MIT license (see LICENSE file).

There are numerous subsystems in various states of maturity.  At
present, the most usable is a web-based interface using CGI.  The
source code for this is found in the following sub-directories:

- pml - the PML language compiler and analysis tools.  

  pml/pml/libpml.a is required to build the PEOS "kernel"

- os/kernel - the PEOS "kernel".  This directory contains the core
  enactment engine that computes process state based on events
  submitted by users, or detected as a result of resource state
  changes.

  os/kernl/libpeos.a is required to build the web ui.

- ui/web2 - this is the original web interface shown in most of the
  PEOS research papers.

- ui/ajax-cgi - this is a work in progress comprising a CGI program
  that recomputes process state in response to events, then returns
  the state of all process instances in XML format.  A simple ajax
  front end shows how to use these data (under construction).

# Installation

To build and install these subsystems, follow these steps:

1. Install library dependencies
   - TCL (see http://www.activestate.com/activetcl).
   - check (http://check.sourceforge.net/).  Check is only required to
   run the unit tests, but the kernel build will fail if check is not
   present. 
   - libxml2 from (http://www.xmlsoft.org/)

2. If the header and libary files  are not installed in the normal 
directory expected by the compiler on your platform (
_/usr/include_ and _/usr/lib_ on Linux platforms),
edit the following Makefiles to set _CFLAGS_ and _LDFLAGS_ to point to
the include and library directories.

   - os/kernel/Makefile (TCL and check)
   - ui/web2/cgi-bin/Makefile (TCL and libxml2)
   - ui/ajax-cgi/Makefile (TCL)


3. Execute _make_ in the root directory (where this _README_ file is
found).  This will invoke _make_ recursively in _pml_, _os/kernel_,
_ui/web2_, and _ui/ajax-cgi_.  

4. Edit _ui/web2/Makefile_ and/or _ui/ajax-cgi/Makefile_  to
set the installation directory for the CGI program(s), html, and
javascript files.  The default is _$(HOME)/public_html/PEOS_ which is
appropriate for most Linux installations.

5. Change directory to _ui/web2/_ and/or _ui/ajax-cgi/_ and
type "make install"; this will install the CGI program(s), html,
javascript, and other configuration files needed to run the PEOS web
application.

The build has been tested on Ubuntu 12.04 with TCL v. 8.5.  It may
work on other systems as 

# Concept of Operations

A model of a _process_ comprises a number of _actions_ that represent
tasks that should be performed to achieve some objective.  Each action
has two fields (_requires_ and _provides_) that specify the
inputs or pre-conditions required for the action to  be performed, and
the outputs or post-conditions that will be true when the action is
completed. 

Actions go through a set of states in response to events:

~~~~~ {.dot}
digraph pml_states {
  None -> Ready {label = "requires true && predecessor DONE"};
  None -> Blocked {label = "requires false && predecessor DONE"};
  None -> Available {label = "requires true"};
  Blocked -> Ready {label = "requires true"};
  Ready -> Active {label = "event = START"};
  Available -> Active {label = "event = START"};
  Active -> Suspended {label = "event = SUSPEND"};
  Suspended -> Active {label = "event = START"};
  Active -> Pending {label = "event = DONE"};
  Pending -> Done {label = "provides true"};
  None -> Satisfied {label = "provides true"};
  Available -> Satisfied {label = "provides true"};
  Ready -> Satisfied {label = "provides true"};  
  Satisfied -> Done {label = "event = DONE"};
}
~~~~~

## Resource Handling

The data or objects that represent inputs and outputs are
called resources; the _requires_ and _provides_ fields can have 
predicates that specify the state of resources necessary for the
action to begin (for _requires_), and the new state of resources when
the action completes.

For example, the following action specifies a simple "test" action:

    action test {
      requires { code.compiles == "true" }
      provides { test_report }
    }

This means that the "test" action requires  code that compiles, and
provides a new item "test_report" when the action is complete.

The kernel will examine the state of all required and provided
resources of all actions in a process before updating their state.
Actions where the resources are available in the state specified by
their _requires_ predicates will be marked "ready" or "available"
depending on the state of the previous action.  Actions where the
resources are in the state specified in the _provides_ predicate will
be marked "done" or "satisfied" depending on whether the actor has
submitted a "DONE" event on the action.  

At the time a process is created, each resource name is bound to a
_value_.  Values are strings that uniquely identify a resource
_instance_.  Examples include filenames for resources that are files,
urls for items in the World Wide Web,
unique keys for relational database entries, or object ids for blobs
in "nosql" stores.

The kernel evaluates each _requires_ and _requires_ predicate by
querying the filesystem or database for the state of the resource.  If
the resource specification does not include a predicate on the
resource's attributes, the kernel just verifies that it exists.  

By default, resources are stored in the filesystem, and resource names
are bound to filenames.  So, in the example above, if _test_report_ is
bound to "peos.tr" then

    provides { test_report }
    
means, "check the filesystem for a file called 'peos.tr.'"

If there is a predicate included, the kernel will evaluate this
predicate.  For example, if _code_ is bound to to "peos.c" in the
example above, then

    requires { code.compiles == "true" }
    
means, "determine whether 'peos.c' compiles without error."

To compute resource state, the kernel relies on a set of resource
scripts defined in the file _peos_init.tcl_.  This file contains
procedures written in the [TCL](http://www.tcl.tk/) scripting language.

The default _peos_init.tcl_ contains two procedures to check if a
resource exists and evaluate a predicate:

~~~~~ {.tcl}
    proc exists { path } {
        # Test whether 'path' is bound to a value.
        if {[catch {set r $path}]} {
            return 0
        }
        # Check if file specified by 'path' exists.
        expr [file exists $path]
    }
    
    # Test whether the value specified in 'v' represents "truth" in TCL terms.
    proc isTrue { v } {
      return [expr {![string is false ${v}]}]
    }
~~~~~

The 'exists' procedure would have to be redefined to work with
resources stored in a database.

### Resource attributes and predicates

A resource specification such as 

    requires { code.compiles == "true" }
    
tells the kernel, "first verify that the resource bound to 'code'
exists, then evaluate its 'compiles' attribute and compare it to the
value 'true.'"  How does the kernel do this?

Verifying that a resource exists is achieved via the _exists_
procedure defined above.  Evaluating resource attributes is similar.

From the kernel's point of view, all resource attributes are
"computed": each attribute has a corresponding TCL procedure that
returns the value of the attribute.  So, for example, the _compiles_
attribute of a file might be implemented with the following TCL
procedure:

~~~~~ {.tcl}
    # Compute 'compiles' attribute by looking for an executable that
    # is derived from, and newer than, the source file specified in 'path'.
    proc compiles { path } {
        # Get executable name ('file rootname' is like unix 'basename')
        set executable [ file rootname $path ]
	# Verify that (1) executable exists, and (2) that it is newer
	# than source.
	return [expr { [file exists $executable] && ([file mtime $executable] > [file mtime $path])}]
    }
~~~~~ 

_Every_ attribute found in a PML specification must have a
corresponding TCL procedure in order for the kernel to successfully
evaluate resource states.  This means, for example, that if you want
your kernel to use a database for resources, you must provide a new
_exists_ procedure, and a procedure for *each* attribute that is used
in *any* of the models you intend to enact.  Fortunately, TCL has many
libraries for accessing repositories in addition to the filesystem,
including [relational databases](http://wiki.tcl.tk/20343),
[XML](http://wiki.tcl.tk/1950), and even
[HL7](https://github.com/jamerfort/tclhl7) (see also
the [TCL wiki entry on HL7](http://wiki.tcl.tk/8893)).
