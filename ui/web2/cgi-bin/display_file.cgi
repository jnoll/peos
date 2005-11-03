#!/bin/sh
echo "Content-type: text/plain"
echo ""

if [ -f $QUERY_STRING ]
then
  echo "##########################################################"
  echo "#                                                        #"
  echo "#                Resource Content (File)                 #"
  echo "#                                                        #"
  echo "##########################################################"
  echo ""  
  cat $QUERY_STRING
elif [ -d $QUERY_STRING ]
then
  echo "##########################################################"
  echo "#                                                        #" 
  echo "#              Resource Content (Directory)              #"
  echo "#                                                        #"
  echo "##########################################################"
  echo ""  
  ls $QUERY_STRING
else
  echo "##########################################################"
  echo "#                                                        #" 
  echo "#             Such resource does not exist.              #"
  echo "#                                                        #"
  echo "##########################################################"
  echo "" 
fi
