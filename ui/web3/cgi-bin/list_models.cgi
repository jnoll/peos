#!/bin/sh
# $Id: list_models.cgi,v 1.1 2007/04/29 07:14:45 jnoll Exp $ 
# Generate clickable process model list; interface for instantiating process instances. 

printf "Content-type: text/html\r\n"
printf "\r\n"

# Get list of pml files.
models=`ls *.pml`

printf "<html>\n<head>\n<body>\n"
printf "<h1>Create Process Instance</h1>"

printf "<p>Click on one of the following model names to create an instance of the model.</p>\n"

printf "<ol>\n"
for m in $models; do
    model=`basename $m .pml`
    printf "<li><a href=\"create.cgi?model=$model\">$model</a></li>\n"
done
printf "</ol>\n"

printf "</body>\n</head>\n</html>\n"

echo "<pre>"
echo "status: $? result: $result"
echo "</pre>"
