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
resource names are unbound: they don't refer to any real-world
object.  The kernel provides a facility (_set_resource_binding()_) for
*binding* resources to *values*.  This allows client programs to
specify what real-world objects resource names refer to, by binding
the resource name to an object's identifier.  Identifiers can be any
ASCII string: a file path, database key, url, or any string that
uniquely identifies an object within the context that the process is
executing.   Each process instance has its own set of bindings, to the
same resource name can refer to two different objects in two different
instances. 

Once a resource name is bound to a value, the kernel can evaluate
requires and provides predicates specified in the model.  This is
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







