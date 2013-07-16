% PEOS Kernel
% John Noll
% Tuesday, July 09 2013  15:09

# Resources

The PEOS resource binding and evaluation mechanism is an important
part of the kernel as it determines which actions are READY,
AVAILABLE, or BLOCKED.

Resources can be thought about at three levels of abstraction.  At the
highest level, resources are referred to by name in PML models; in
this way, one can specify how resources flow through the actions of a
model without being concerned with the real-world objects (files,
database records, documents,  web pages, etc.) to which these names refer.

When the kernel creates a running instance of a process model, the
resource names are unbound: they don't refer to any real-world object.
The kernel provides a facility (_set_resource_binding()_) for
*binding* resources to *values*.  This allows client programs to
specify what real-world objects resource names refer to, by binding
the resource name to an object's identifier.  Object identifiers can
be any ASCII string: a file path, database key, url, or any string
that uniquely identifies an object within the context that the process
is executing.  Each process instance has its own set of bindings, to
the same resource name can refer to two different objects in two
different instances.



Once a resource name is bound to a value, the kernel can evaluate
_requires_ and _provides_ predicates specified in the model.  This is
achieved with the aid of Tool Command Language (TCL) commands that are
specified in the _peos_init.tcl_ file.  

As an example, consider the following PML fragment:

~~~~~
process test {
    action compile {
        requires { source_code.exists == "True" }
	provides { executable.exists == "True" }
    }

    action run {
        requires { executable.exists == "True" }
    }
}
~~~~~

When an instance of this model is created, the names _source_code_ and
_executable_ are unbound: they don't refer to anything.   The client
program might bind these resource names as follows:

~~~~~
	set_resource_binding(pid, "source_code", "foo.c");
	set_resource_binding(pid, "executable", "foo.exe");
~~~~~

At this point, the process *state* has not been updated.  When the
client then calls any function that causes the kernel to update the
process state (_peos_notify()_ or _peos_list_attributes()_), 
the kernel will evaluate the _requires_ and _provides_ predicates
of each action as follows 


1. Retrieve that value bound to each resource name and set the value
of a TCL variable accordingly.
2. Evaluate expressions of the form "resource.attribute" by executing 
a TCL command:

    attribute ${resource}

3. Evaluate the resource predicate using the result obtained from
executing the TCL command.

For the example above, to evaluate the _compile_ action's _requires_
predicate, the kernel would:

1. set a TCL variable _source_code_ to "foo.c"

    set source_code foo.c

2. evaluate the _exists_ attribute:  (_exists_ is a TCL procedure
defined in _peos_init.tcl_)

    exists ${source_code}

3. Compare the result as a boolean value to the value "True."

The kernel implements some default expressions to make modeling
easier.  If there is no comparison operator in an expression, then `==
"True"` is assumed.  So,

    source_code.exists == "True" 

can be written as 

    source_code.exists

Also, if an expression contains neither an attribute nor an operator,
_exists_ is assumed; so, 

    source_code.exists == "True" 

is equivalent can be written as just:

    source_code


In order for this mechanism to work, the following conditions must be
satisfied:

1. Resource names must be bound to values.  If they are not, the
predicate will automatically be false.
2. There must be a corresponding TCL command for *every* attribute; if
this is not the case, the kernel will return an error (this is a bug).

## Derived Resource Bindings

The kernel also allows resources to be bound to *expressions*, so that
resource values can be derived at runtime.  For example, the web test
process (_models/web_test.pml_) has an action to create the working
directory, and a subsequent action to create a _Makefile_ and _README_ file:

~~~~~
action create_working_directory {
    /* requires nothing */
    provides { working_directory }
}

action create_makefile {
    requires { working_directory }
    provides { Makefile }
}

action create_readme {
    requires { working_directory }
    provides { readme }
}

~~~~~

The value of the  _working_directory_ resource can be used to derive
the values of the _Makefile_ and _readme_ resources:

    Makefile : ${working_directory}/Makefile
    readme : ${working_directory}/README

These expressions should be written in a file with the model name as
basename, and _.res_ extension.  Then, when an instance of the model
is created, the kernel will look for a an associated _.res_ file and
create any resource bindings found there; then, these resource binding
expressions will be evaluated before _requires_ and _provides_
predicates are evaluated to determine the state of actions.

Note: resource value expressions must be valid _TCL_ expressions; the
kernel does not (at present) understand PML resource expressions.  So,
if you want to bind a resource to another resource's attribute, you
must write `[attr ${resource}]` rather than `resource.attr`.  For
example, suppose an action requires a "patient history"  resource:

~~~~~
action review_history
    requires { patient_history }
}
~~~~~

Now, we want this to be bound to the _patient_history_ attribute of an
electronic medical record ; the way to express this in a _.res_ file
is:

    patient_history : [patient_history ${emr}]

Then, at runtime, the _patient_history_ resource will be bound to
_emr.patient_history_.  In this way, a client program only needs to
specify a value for _emr_, and other resources will be derived from
this value.





