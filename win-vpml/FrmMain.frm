VERSION 5.00
Begin VB.Form FrmMain 
   BackColor       =   &H00FFFFFF&
   Caption         =   "Using PML to Create Processes"
   ClientHeight    =   7185
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   8070
   LinkTopic       =   "Form1"
   ScaleHeight     =   7185
   ScaleWidth      =   8070
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      BackColor       =   &H00FFFFFF&
      Caption         =   "PML Tools"
      Height          =   855
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   3255
      Begin VB.Image ImAction 
         Height          =   480
         Left            =   2640
         Picture         =   "FrmMain.frx":0000
         Top             =   240
         Width           =   480
      End
      Begin VB.Image ImIteration 
         Height          =   480
         Left            =   1440
         Picture         =   "FrmMain.frx":030A
         Top             =   240
         Width           =   480
      End
      Begin VB.Image ImSelection 
         Height          =   480
         Left            =   2040
         Picture         =   "FrmMain.frx":0614
         Top             =   240
         Width           =   480
      End
      Begin VB.Image ImBranch 
         Height          =   480
         Left            =   240
         Picture         =   "FrmMain.frx":091E
         Top             =   240
         Width           =   480
      End
      Begin VB.Image ImSequence 
         Height          =   480
         Left            =   840
         Picture         =   "FrmMain.frx":11E8
         Top             =   240
         Width           =   480
      End
   End
   Begin VB.Line Lines 
      Index           =   1
      Visible         =   0   'False
      X1              =   7680
      X2              =   7920
      Y1              =   5760
      Y2              =   6000
   End
   Begin VB.Image ImActNorm 
      Height          =   480
      Left            =   7560
      Picture         =   "FrmMain.frx":14F2
      Top             =   5040
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image ImActSel 
      Height          =   480
      Left            =   7440
      Picture         =   "FrmMain.frx":17FC
      Top             =   4440
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image ImSelNorm 
      Height          =   480
      Left            =   7560
      Picture         =   "FrmMain.frx":1B06
      Top             =   3840
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image ImSelSel 
      Height          =   480
      Left            =   7560
      Picture         =   "FrmMain.frx":1E10
      Top             =   3240
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image ImIterNorm 
      Height          =   480
      Left            =   7440
      Picture         =   "FrmMain.frx":211A
      Top             =   2640
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image ImIterSel 
      Height          =   480
      Left            =   7440
      Picture         =   "FrmMain.frx":2424
      Top             =   1920
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image ImSeqNorm 
      Height          =   480
      Left            =   7320
      Picture         =   "FrmMain.frx":272E
      Top             =   1440
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image ImSeqSel 
      Height          =   480
      Left            =   7440
      Picture         =   "FrmMain.frx":2A38
      Top             =   960
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image ImbraNorm 
      Height          =   480
      Left            =   7320
      Picture         =   "FrmMain.frx":2D42
      Top             =   480
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image ImBraSel 
      Height          =   480
      Left            =   7320
      Picture         =   "FrmMain.frx":360C
      Top             =   0
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image ImProcess 
      DragMode        =   1  'Automatic
      Height          =   480
      Index           =   1
      Left            =   3960
      Picture         =   "FrmMain.frx":3ED6
      Top             =   360
      Width           =   480
   End
   Begin VB.Menu mnuFile 
      Caption         =   "File"
      Begin VB.Menu mnuFileOpen 
         Caption         =   "Open Model"
      End
      Begin VB.Menu mnuFileSave 
         Caption         =   "Save Model"
      End
      Begin VB.Menu mnuFileCreatePML 
         Caption         =   "Create PML File"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "Exit"
      End
   End
End
Attribute VB_Name = "FrmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public Which_Image As String
Public Screen_Active As Boolean
Dim aPicts() As PictObject
Dim iCurX As Integer
Dim iCurY As Integer
'Dim ObjCounter As Integer      Moved to global vars
Dim CurrentTool As Integer     'Use this to know what type object to add
Dim Indent As Integer

'Contants
Dim constObjSpacing As Integer
' +------------------------------------------------------------------------+
' |                            VPML Over View                              |
' |                                                                        |
' |  General Overview:  The Vpml program assist the user in building a form|
' |                     visually to represent a PML process description.   |
' |                     The form consists of a number of randomly placed   |
' |                     icons.  The user selects the type of icon they want|
' |                     and adds it to the existing process.  Once added to|
' |                     to the process the icon can be dragged and dropped |
' |                     anywhere on the form.  Whenever an icon is dragged |
' |                     dropped its sub-tree will re-align its self.  Icons|
' |                     can be dragged an dropped on another segment of the|
' |                     process tree, thus allowing any sub-tree to be     |
' |                     re-posionted under any existing sub-tree.          |
' |                                                                        |
' |  Operational Concept:  The operation of the program is described in the|
' |                        state machine in the op-Concept document.       |
' |                                                                        |
' |  Implementation:                                                       |
' |     Data:  To understand the data structures used in this program it is|
' |            very important to understand the index.  The icons on the   |
' |            form are implemented as an array.  The index to the array is|
' |            consistant across all data items.  We refer to this index as|
' |            the aPicts index.                                           |
' |                                                                        |
' |            The form contains a array of icons displayed on the screen. |
' |            Each icons index in its tag field.                      |
' |                                                                        |
' |            There is a data array called aPicts.  The data in this array|
' |            contains information on visual attributes of each icon.  All|
' |            of this information is used to draw the tree.               |
' |                                                                        |
' |            There is a line array used to connect the icons visually.   |
' |                                                                        |
' |            There ia a Access database by the name of PmlData.mdb.  This|
' |            is used to store PML data, and gives persistance to the     |
' |            modeled process.  Items in this database have a field named |
' |            aPicts. This is the index discussed above.                  |
' |                                                                        |
' |     Data Scheme:                                                       |
' |            The process is built in terms of a tree.  At any level of   |
' |            tree there can be an unlimited number of children.  Because |
' |            the number of children are varible the recursive routines   |
' |            used to redraw, and traverse the tree are not as straight   |
' |            forward as desirable.  Each object knows its parent using   |
' |            ParentObj used in the aPicts array.  Each object knows its  |
' |            children by scanning its sibling array (mis-named) in the   |
' |            aPicts array.                                               |
' |                                                                        |
' |     Current Deficiencies:                                              |
' |            1) Althought Data for the object is loaded into a persistant|
' |               data store, there is no way to save or load diagrams     |
' |               generated by this program.                               |
' |            2) The objects on the form need to be labeled (visually).   |
' |               There is a name label in the aPicts array for each object|
' |               , however these are not displayed on the form by the     |
' |               object.                                                  |
' |            3) When a user Drages a sub-tree and reconnects it somewhere|
' |               else on the tree visually this process works well.  When |
' |               the PML file is generated however, the sub-tree elements |
' |               are generated in both places (a BUG).
' |            4) Error detection on data entry is poor.  Currently it is  |
' |               easy to crash the program here.  Also PML as defined will|
' |               not allow object name with spaces embedded, this program |
' |               currently will allow the user to do this.                |
' |                                                                        |
' |     Suggested Upgrades:                                                |
' |            1) After using this program to write documentation it is    |
' |               very evident that the program would be much more powerful|
' |               if a user could load a saved process (see defeciency #1) |
' |               below any object on the form (simular to Drag'n Drop).   |
' |               This would promote reuse in a manner simular to design   |
' |               patterns.                                                |
' +------------------------------------------------------------------------+




Private Sub Form_DblClick()
    Dim icounter As Integer
    
    For icounter = 0 To 99
        If iCurX >= aPicts(icounter).x1 And iCurX <= aPicts(icounter).x2 And _
            iCurY >= aPicts(icounter).y1 And iCurY <= aPicts(icounter).y2 Then
                MsgBox (aPicts(icounter).obj_name)
        End If
    Next icounter
    
End Sub

Private Sub Form_DragDrop(Source As Control, x As Single, Y As Single)
'   This routine is used when the user repositions an icon on the form
'   The sub-tree is recursivly re-positioned.
Dim CO As Integer   'The current object
Dim po As Integer   'The parent object
Dim count  As Integer
    Source.Left = x
    Source.Top = Y
    CO = Source.Tag
    po = aPicts(CO).ParentObj
    aPicts(CO).ScreenLocAbsoluteLeft = x
    aPicts(CO).ScreenLocAbsoluteTop = Y
    If x < aPicts(CO).ScreenLocAbsoluteLeft Then
        aPicts(CO).ScreenLocRelativeLeft = x - aPicts(po).ScreenLocAbsoluteLeft
    Else
        aPicts(CO).ScreenLocRelativeLeft = aPicts(po).ScreenLocAbsoluteLeft - x
    End If
    If Y < aPicts(CO).ScreenLocAbsoluteTop Then
        aPicts(CO).ScreenLocRelativeTop = Y - aPicts(po).ScreenLocAbsoluteTop
    Else
        aPicts(CO).ScreenLocRelativeTop = aPicts(po).ScreenLocAbsoluteTop - Y
    End If
    If CO > 1 Then
        Lines(CO).x2 = aPicts(CO).ScreenLocAbsoluteLeft + (ImProcess(CO).Width / 2)
        Lines(CO).y2 = aPicts(CO).ScreenLocAbsoluteTop + (ImProcess(CO).Height / 2)
        Lines(CO).x1 = aPicts(po).ScreenLocAbsoluteLeft + (ImProcess(po).Width / 2)
        Lines(CO).y1 = aPicts(po).ScreenLocAbsoluteTop + (ImProcess(po).Height / 2)
    End If
    'Recursivly update the subtrees of co
    For count = 1 To aPicts(CO).SiblingCount
        Redraw (aPicts(CO).SiblingObj(count))
    Next count
        
End Sub

Private Sub Form_Load()
'   Initializes the form to begin a new modeling
    constObjSpacing = 750
    If Right(App.Path, 1) = "\" Then
        glbACCESSPMLPath = App.Path & "PMLData.mdb"
    Else
        glbACCESSPMLPath = App.Path & "\PMLData.mdb"
    End If
    InitVariables
    
    Screen_Active = False
    ReDim aPicts(100) As PictObject
    'Init the root of the list to be the process icon
    ObjCounter = 1
    aPicts(1).obj_name = "Process"
    aPicts(1).obj_type = "Process"
    aPicts(1).x1 = ImProcess(1).Left
    aPicts(1).y1 = ImProcess(1).Top
    aPicts(1).ScreenLocAbsoluteLeft = ImProcess(1).Left
    aPicts(1).ScreenLocAbsoluteTop = ImProcess(1).Top
    aPicts(1).ScreenLocRelativeLeft = 0
    aPicts(1).ScreenLocRelativeTop = 0
    aPicts(1).SiblingCount = 0
    ImProcess(1).Tag = 1
    CurrentTool = -1
End Sub

Private Sub Form_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
' Currently this code is obsolete.
    Dim Pict As Picture
    Dim StrPict As String
    Dim BoolOktoPlace As Boolean
    Dim icounter As Integer
    
    iCurX = x
    iCurY = Y

    
    If Screen_Active = True Then
        If ObjCounter > 99 Then
            MsgBox "Max number of objects reached."
        Else
            'Check if object already there if so do not place object.
            BoolOktoPlace = True
            For icounter = 0 To 99
                If iCurX >= aPicts(icounter).x1 And iCurX <= aPicts(icounter).x2 And _
                    iCurY >= aPicts(icounter).y1 And iCurY <= aPicts(icounter).y2 Then
                        MsgBox (aPicts(icounter).obj_name)
                        BoolOktoPlace = False
                        Exit For
                End If
            Next icounter
            
            If BoolOktoPlace = True Then
                StrPict = "a:\CSC5728\" & Which_Image & ".ico"
                Set Pict = LoadPicture(StrPict)
            
                'Me.PaintPicture Pict, X, Y
                Me.PaintPicture Pict, (aPicts(ObjCounter).x1), (aPicts(ObjCounter).y1 + constObjSpacing)
                Screen_Active = False
                aPicts(ObjCounter + 1).obj_name = ObjCounter
                aPicts(ObjCounter + 1).obj_type = Which_Image
                aPicts(ObjCounter + 1).x1 = (aPicts(ObjCounter).x1)
                aPicts(ObjCounter + 1).x2 = x + Pict.Width
                aPicts(ObjCounter + 1).y1 = (aPicts(ObjCounter).y1 + constObjSpacing)
                aPicts(ObjCounter + 1).y2 = Y + Pict.Height
                ObjCounter = ObjCounter + 1
            End If
        End If
    End If
        
End Sub

Private Sub ImAction_Click()
'   The user has selected the Action icon on the PML Tool bar
    Which_Image = "Action"
    ImAction.Picture = ImActSel.Picture
    CurrentTool = 2
    Screen_Active = False
End Sub

Private Sub ImBranch_Click()
'   The user has selected the Branch icon on the PML tool bar
    Which_Image = "Branch"
    CurrentTool = 4
    ImBranch.Picture = ImBraSel.Picture
    Screen_Active = False
End Sub

Private Sub ImIteration_Click()
'   The users has selected the Iteration icon on the PML Tool bar
    Which_Image = "Iterate"
    ImIteration.Picture = ImIterSel.Picture
    CurrentTool = 6
    Screen_Active = False
End Sub

Private Sub ImProcess_Click(Index As Integer)
'   The user has selected an icon in the current PML Process.  If a Tool bar
'   icon has been selected then add it a child to this (Index) icon. If no
'   Tool bar icon is currently selected  then link to a data entry form for
'   PML data.
    If CurrentTool = -1 Then
        Select Case aPicts(Index).obj_type
            Case "Process   "
                    glbWhich_image = aPicts(Index).obj_type
                    glbIndex = Index
                    FrmProcess.Show vbModal
            Case "Action    "
                    glbWhich_image = aPicts(Index).obj_type
                    glbIndex = Index
                    FrmAction.Show vbModal
            Case Else
                    glbWhich_image = aPicts(Index).obj_type
                    glbIndex = Index
                    frmItSeqBrSel.Show vbModal
                   
        End Select
        CurrentTool = -1
     Else
    
        Which_Image = "Process"
        ' we need to add code for adding to an item in the tree here
        Load ImProcess(ObjCounter + 1)      'Create a new Icon
        ImProcess(ObjCounter + 1).Left = ImProcess(Index).Left
        ImProcess(ObjCounter + 1).Top = ImProcess(Index).Top + constObjSpacing
        ImProcess(ObjCounter + 1).Visible = True
        ImProcess(ObjCounter + 1).Tag = ObjCounter + 1
        Load Lines(ObjCounter + 1)
        Lines(ObjCounter + 1).x1 = ImProcess(Index).Left + (ImProcess(Index).Width / 2)
        Lines(ObjCounter + 1).y1 = ImProcess(Index).Top + (ImProcess(Index).Height / 2)
        Lines(ObjCounter + 1).x2 = ImProcess(ObjCounter + 1).Left + (ImProcess(ObjCounter + 1).Width / 2)
        Lines(ObjCounter + 1).y2 = ImProcess(ObjCounter + 1).Top + (ImProcess(ObjCounter + 1).Height / 2)
        Lines(ObjCounter + 1).Visible = True
    
        If CurrentTool = 2 Then
            'Add a Action to the screen
            ImProcess(ObjCounter + 1).Picture = ImActNorm.Picture
            ImAction.Picture = ImActNorm.Picture
            aPicts(ObjCounter + 1).obj_type = "Action"
        End If
        If CurrentTool = 3 Then
            'Add a Sequence to the screen
            ImProcess(ObjCounter + 1).Picture = ImSeqNorm.Picture
            ImSequence.Picture = ImSeqNorm.Picture
        aPicts(ObjCounter + 1).obj_type = "Sequence"
        End If
        If CurrentTool = 4 Then
            'Add a Branch to the screen
            ImProcess(ObjCounter + 1).Picture = ImbraNorm.Picture
            ImBranch.Picture = ImbraNorm.Picture
            aPicts(ObjCounter + 1).obj_type = "Branch"
        End If
          If CurrentTool = 5 Then
            'Add a Selection to the screen
            ImProcess(ObjCounter + 1).Picture = ImSelNorm.Picture
            ImSelection.Picture = ImSelNorm.Picture
            aPicts(ObjCounter + 1).obj_type = "Selection"
        End If
        If CurrentTool = 6 Then
            'Add a Iterator to the screen
            ImProcess(ObjCounter + 1).Picture = ImIterNorm.Picture
            ImIteration.Picture = ImIterNorm.Picture
            aPicts(ObjCounter + 1).obj_type = "Iterator"
        End If
 
        'Setup the aPict array item
        aPicts(ObjCounter + 1).obj_name = "Undefined"
        aPicts(ObjCounter + 1).ParentObj = ImProcess(Index).Tag
        aPicts(ObjCounter + 1).ScreenLocAbsoluteLeft = ImProcess(ObjCounter + 1).Left
        aPicts(ObjCounter + 1).ScreenLocAbsoluteTop = ImProcess(ObjCounter + 1).Top
        aPicts(ObjCounter + 1).ScreenLocRelativeLeft = 0
        aPicts(ObjCounter + 1).ScreenLocRelativeTop = constObjSpacing * -1
        'Add element to the siblig list of the image receiving the drop
        With aPicts(Index)
            .SiblingCount = .SiblingCount + 1
            ReDim Preserve .SiblingObj(.SiblingCount)
            .SiblingObj(.SiblingCount) = ObjCounter + 1
        End With
    
        ObjCounter = ObjCounter + 1
        CurrentTool = -1
        Screen_Active = False
    End If
End Sub

Private Sub ImProcess_DragDrop(Index As Integer, Source As Control, x As Single, Y As Single)
'   The user has taken an item on the current tree and dropped it here.
    ' Index is the index of the item bieng droped on
    ' Source is the item being dropped
    Dim i As Integer
    Dim CO As Integer   'The current object
    Dim po As Integer   'The parent object
    Dim count  As Integer
    
   If (Index <> Source.Tag) And (aPicts(Index).obj_type <> "Action    ") Then
    
       ' add dropped item to child list of the index
       With aPicts(Index)
            .SiblingCount = .SiblingCount + 1
            ReDim Preserve .SiblingObj(.SiblingCount)
            .SiblingObj(.SiblingCount) = Source.Tag
       End With
    
        ' remove the dropped item from its old parents child list
        For i = 1 To aPicts(aPicts(Source.Tag).ParentObj).SiblingCount
            If aPicts(aPicts(Source.Tag).ParentObj).SiblingObj(i) = Source.Tag Then
                aPicts(aPicts(Source.Tag).ParentObj).SiblingObj(i) = 0
            End If
        Next i
        ' update the dropped items parent
        aPicts(Source.Tag).ParentObj = Index
        
        ' redraw
        Source.Left = aPicts(aPicts(Source.Tag).ParentObj).ScreenLocAbsoluteLeft
        Source.Top = aPicts(aPicts(Source.Tag).ParentObj).ScreenLocAbsoluteTop + constObjSpacing
        CO = Source.Tag
        po = aPicts(CO).ParentObj
        aPicts(CO).ScreenLocAbsoluteLeft = Source.Left
        aPicts(CO).ScreenLocAbsoluteTop = Source.Top
        aPicts(CO).ScreenLocRelativeLeft = 0
        aPicts(CO).ScreenLocRelativeTop = constObjSpacing * -1
        If CO > 1 Then
            Lines(CO).x2 = aPicts(CO).ScreenLocAbsoluteLeft + (ImProcess(CO).Width / 2)
            Lines(CO).y2 = aPicts(CO).ScreenLocAbsoluteTop + (ImProcess(CO).Height / 2)
            Lines(CO).x1 = aPicts(aPicts(CO).ParentObj).ScreenLocAbsoluteLeft + (ImProcess(aPicts(CO).ParentObj).Width / 2)
            Lines(CO).y1 = aPicts(aPicts(CO).ParentObj).ScreenLocAbsoluteTop + (ImProcess(aPicts(CO).ParentObj).Height / 2)
        End If
        'Recursivly update the subtrees of co
        For count = 1 To aPicts(CO).SiblingCount
            Redraw (aPicts(CO).SiblingObj(count))
        Next count
    End If
End Sub

Private Sub ImSelection_Click()
'   The user has seleceted a selection on the PML Tool bar
    Which_Image = "Select"
    ImSelection.Picture = ImSelSel
    CurrentTool = 5
    Screen_Active = False
End Sub

Private Sub ImSequence_Click()
' The user has selected the sequence icon on the PML Tool bar
    Which_Image = "Sequence"
    ImSequence.Picture = ImSeqSel.Picture
    CurrentTool = 3
    Screen_Active = False
End Sub


Private Function Redraw(Obj As Integer)
'   The user has repostioned an icon. Recursivly call this routine to re-position
'   the PLM icons on the screen
    Dim x As Integer
    Dim CurrentObj As Integer
    CurrentObj = Obj
    'Move object to new relitive address
    If CurrentObj > 1 Then
        aPicts(CurrentObj).ScreenLocAbsoluteLeft = aPicts(aPicts(CurrentObj).ParentObj).ScreenLocAbsoluteLeft - aPicts(CurrentObj).ScreenLocRelativeLeft
        aPicts(CurrentObj).ScreenLocAbsoluteTop = aPicts(aPicts(CurrentObj).ParentObj).ScreenLocAbsoluteTop - aPicts(CurrentObj).ScreenLocRelativeTop
        ImProcess(CurrentObj).Move aPicts(CurrentObj).ScreenLocAbsoluteLeft, aPicts(CurrentObj).ScreenLocAbsoluteTop
        Lines(CurrentObj).x2 = aPicts(CurrentObj).ScreenLocAbsoluteLeft + (ImProcess(CurrentObj).Width / 2)
        Lines(CurrentObj).y2 = aPicts(CurrentObj).ScreenLocAbsoluteTop + (ImProcess(CurrentObj).Height / 2)
        Lines(CurrentObj).x1 = aPicts(aPicts(CurrentObj).ParentObj).ScreenLocAbsoluteLeft + (ImProcess(aPicts(CurrentObj).ParentObj).Width / 2)
        Lines(CurrentObj).y1 = aPicts(aPicts(CurrentObj).ParentObj).ScreenLocAbsoluteTop + (ImProcess(aPicts(CurrentObj).ParentObj).Height / 2)
        'Now move siblings
        For x = 1 To aPicts(CurrentObj).SiblingCount
            Redraw (aPicts(CurrentObj).SiblingObj(x))
        Next x
    End If
End Function

Private Sub mnuFileCreatePML_Click()
    ' This routine creates a PML file form the existing model.
    ' The file is named "Process Name".pml, where "Process Name" is
    ' the name of the process element in the diagram.
    
    ' Check to make sure the process has a name
    Dim Indent As String
    Dim x As Integer
    
    Indent = ""
    If txtGlbProcessName = "" Then
        MsgBox ("Cannot create a PML file for an unnamed Process, Please name the Process.")
    Else
 MsgBox ("writing to " & App.Path & "\" & txtGlbProcessName & ".pml")
       Open (App.Path & "\" & txtGlbProcessName & ".pml") For Output As #1
       x = PrintPML("", 1)
       Close #1
    End If
End Sub

Private Function PrintPML(Indent As String, Obj As Integer)
'Private Function PrintPML(Obj As Integer)
    Dim str As String
    Dim astr As String
    Dim name As String
    Dim i As Integer
    Dim ActionType As String
    Dim SpecType As String
    Dim Selector As String
    Dim ASelector As String
    
    If (Obj <> 0) Then
        Dim CurrentIndent As String
        Dim CurrentObj As Integer
        Dim SibPos As Integer
        CurrentObj = Obj
    
        CurrentIndent = Indent + "   "
        str = "select * from PMLInfo where ProcessName = '" & txtGlbProcessName & "' and Apictsindex = '" & CurrentObj & "'"
        Set glbObjectInfoTable = glbDB.OpenRecordset(str)
        If glbObjectInfoTable.RecordCount > 0 Then
           name = glbObjectInfoTable!name
        Else
            name = ""
        End If
        Select Case aPicts(CurrentObj).obj_type
            Case "Process   "
                Print #1, "process" & " " & txtGlbProcessName & " {"
            Case "Action    "
               ActionType = glbObjectInfoTable!ActionType
                Print #1, CurrentIndent & "action " & name & "  " & ActionType & " {"
                astr = "select * from PMLInfo where ProcessName = '" & txtGlbProcessName & "' and Apictsindex = '" & CurrentObj & "' and Name= '" & name & "'"
'                MsgBox (astr)
                Set glbActionInfoTable = glbDB.OpenRecordset(astr)
'                MsgBox ("record count= " & glbActionInfoTable.RecordCount)
                While Not glbActionInfoTable.EOF
                
                    SpecType = glbActionInfoTable!SpecType
                    Selector = glbActionInfoTable!Selector
                    Print #1, CurrentIndent & "    " & SpecType & "{""" & Selector & """}"
                    glbActionInfoTable.MoveNext
                    
                Wend
             Case "Branch    "
                Print #1, CurrentIndent & "branch " & name & " {"
            Case "Selection "
                Print #1, CurrentIndent & "selection " & name & " {"
            Case "Iterator  "
                Print #1, CurrentIndent & "iterator " & name & " {"
            Case "Sequence  "
                Print #1, CurrentIndent & "sequence " & name & " {"
            Case Else
        End Select
        
            SibPos = 0
            For i = 1 To aPicts(CurrentObj).SiblingCount
                 Call PrintPML(CurrentIndent, NextSibling(CurrentObj, SibPos))
            Next i
        Print #1, CurrentIndent & "}"
    End If
End Function

Private Function NextSibling(CurrentObj As Integer, LastSibPos As Integer) As Integer
'   This routine gets the next child from the sibling list
'   NOTE the name sibling was incorrectly used.  We are search for child objects.
    Dim x As Integer
    Dim currNext As Integer
    Dim currNextValue As Integer
    Dim thisSibValue As Integer
    
    currNext = aPicts(CurrentObj).SiblingCount
    currNextValue = 30000
    For x = 1 To aPicts(CurrentObj).SiblingCount
        thisSibValue = aPicts(aPicts(CurrentObj).SiblingObj(x)).ScreenLocAbsoluteLeft
        If (thisSibValue > LastSibPos) And (thisSibValue <= currNextValue) Then
            currNextValue = thisSibValue
            currNext = x
        End If
    Next x

    NextSibling = aPicts(CurrentObj).SiblingObj(currNext)
    LastSibPos = aPicts(NextSibling).ScreenLocAbsoluteLeft
End Function
