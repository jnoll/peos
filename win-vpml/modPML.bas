Attribute VB_Name = "PublicVariables"
Type PictObject   ' Create user-defined type.
   x1 As Integer   ' Define elements of data type.
   x2 As Integer
   y1 As Integer
   y2 As Integer
   ScreenLocRelativeLeft As Integer 'When redraw is necessary the
   ScreenLocRelativeTop As Integer  ' relitive position is needed
   ScreenLocAbsoluteLeft As Integer 'For Drag'n Drop the absolute
   ScreenLocAbsoluteTop As Integer  ' address is needed
   SiblingObj() As Integer          'List of encompassed objects
   SiblingCount As Integer          'Number of items in immediate sub-tree
   ParentObj As Integer
   obj_name As String * 50
   obj_type As String * 10
 End Type
 
Public AdoCon As ADODB.Connection

Public glbIndex As Integer
Public glbWhich_image As String
Public glbDB As DAO.Database
Public glbObjectInfoTable As DAO.Recordset
Public glbACCESSPMLPath As String
Public txtGlbProcessName As String
Public ObjCounter As Integer

' Vars for writing the PML file

 Dim fso, glbPMLFile

