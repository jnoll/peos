package Utils;

import java.io.*;

public class PMLException extends IOException {
  public PMLException() { }
  public PMLException(String errorMessage)
  {
    super(errorMessage);
  }
}

