Attribute VB_Name = "PublicFunctions"
Public Sub InitVariables()
    
    glbACCESSPMLPath = App.Path & "\PMLData.mdb"
    Set glbDB = DBEngine.OpenDatabase(glbACCESSPMLPath)
    
    Set glbObjectInfoTable = glbDB.OpenRecordset("PMLInfo")
    
End Sub

Public Sub temp()
    Dim rst As DAO.Recordset
    Set rst = varDB.OpenRecordset("SELECT DISTINCT ERRORS.ERROR_DESC, TRNErrors.Error_Code, TRNFILE.InstCode, TRNFILE.Lab, TRNFILE.Lot, TRNFILE.SampNum, TRNFILE.TestName, TRNFILE.Method, TRNFILE.Boolean, TRNFILE.UncorrValue, TRNFILE.Dilution, TRNFILE.Moisture, TRNFILE.FlagCode, TRNFILE.QCTestCode, TRNFILE.QCSpike, TRNFILE.SampDate, TRNFILE.PrepDate, TRNFILE.AnalDate FROM (ERRORS RIGHT JOIN TRNErrors ON ERRORS.ERROR_NUM = TRNErrors.Error_Code) INNER JOIN TRNFILE ON TRNErrors.[ID#] = TRNFILE.[Record#] Where (((TRNErrors.LotSampAnal) = 'A')) ORDER BY TRNFILE.Lab, TRNFILE.Lot, TRNFILE.SampNum, TRNFILE.TestName;")
End Sub
