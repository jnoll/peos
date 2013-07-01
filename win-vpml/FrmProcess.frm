VERSION 5.00
Begin VB.Form frmProcess 
   BackColor       =   &H0000FFFF&
   Caption         =   "Process info"
   ClientHeight    =   2265
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6720
   LinkTopic       =   "Form1"
   ScaleHeight     =   2265
   ScaleWidth      =   6720
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton Command2 
      Caption         =   "Cancel"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   14.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   3480
      TabIndex        =   3
      Top             =   1560
      Width           =   1455
   End
   Begin VB.CommandButton Command1 
      Caption         =   "OK"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   14.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   1920
      TabIndex        =   2
      Top             =   1560
      Width           =   1455
   End
   Begin VB.TextBox txtprocessname 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2160
      TabIndex        =   0
      Top             =   960
      Width           =   4335
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      BackColor       =   &H0000FFFF&
      Caption         =   "Input a name for this process.  It must be a unique name."
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   14.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   735
      Left            =   1440
      TabIndex        =   4
      Top             =   120
      Width           =   3975
   End
   Begin VB.Label Label1 
      BackColor       =   &H0000FFFF&
      Caption         =   "Process Name:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   14.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   960
      Width           =   2175
   End
End
Attribute VB_Name = "FrmProcess"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()
Dim str As String
    str = "select * from PMLInfo where ProcessName = '" & Me!txtprocessname & "'"
    Set glbObjectInfoTable = glbDB.OpenRecordset(str)
    If glbObjectInfoTable.RecordCount > 0 Then
        If MsgBox("There is already a process with that name.  Would you like to replace it?", vbYesNo) = vbYes Then
            str = "delete * from pmlinfo where processname = '" & Me!txtprocessname & "'"
            glbDB.Execute str
            txtGlbProcessName = Me!txtprocessname
            Unload Me
        Else
            Me!txtprocessname.SetFocus
        End If
        Set glbObjectInfoTable = glbDB.OpenRecordset("PMLInfo")
    Else
        txtGlbProcessName = Me!txtprocessname
        Set glbObjectInfoTable = glbDB.OpenRecordset("PMLInfo")
        Unload Me
    End If
End Sub

Private Sub Command2_Click()
    Unload Me
End Sub
