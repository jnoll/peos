//
// EIUnitTest - unit testing program for engineInterface and PMLServer.
//   writen and document by Ming Rutar
//
Description:
   EIUnitTest runs an infinite loop which randomly creates processes and executes tasks. Press CTRL-C to terminate the program. 

Make:
   Run make. EIUnitTest uses engineInterface package from ?peospath?share/java.

Launch:
   1. If you added ?peospath?/share/java path to $CLASSPATH, type at commandline
	java EIUnitTest name host port

   2. Otherwise at commanline type:
	java -cp ?peospath?share/java EIUnitTest name host port

