VERSION 5.00
Begin VB.Form frmItSeqBrSel 
   BackColor       =   &H8000000A&
   Caption         =   "Iteration,Sequence,Branch, or Selection Info"
   ClientHeight    =   1950
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6900
   LinkTopic       =   "Form1"
   ScaleHeight     =   1950
   ScaleWidth      =   6900
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtprocessname 
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2040
      TabIndex        =   2
      Top             =   720
      Width           =   4695
   End
   Begin VB.CommandButton Command1 
      BackColor       =   &H00008000&
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
      Left            =   1440
      TabIndex        =   1
      Top             =   1200
      Width           =   1455
   End
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
      TabIndex        =   0
      Top             =   1200
      Width           =   1455
   End
   Begin VB.Label Label1 
      Caption         =   "Iteration Name:"
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
      Left            =   0
      TabIndex        =   4
      Top             =   720
      Width           =   2055
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      Caption         =   "Input the name. Then press the 'OK' button."
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
      Left            =   360
      TabIndex        =   3
      Top             =   120
      Width           =   6135
   End
End
Attribute VB_Name = "frmItSeqBrSel"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


Private Sub Command1_Click()
    If txtGlbProcessName = "" Then
        MsgBox "You must first put in the process name.", vbOKOnly
    Else
        With glbObjectInfoTable
            .AddNew
            !processname = txtGlbProcessName
            !objecttype = glbWhich_image
            !Name = Me!txtprocessname
            !Apictsindex = glbIndex
            .Update
        End With
    End If
    Unload Me
End Sub

Private Sub Command2_Click()
    Unload Me
End Sub

Private Sub Form_Load()

    Select Case glbWhich_image
        Case "Iterator  "
            frmItSeqBrSel.BackColor = &H800000
            Me!Label1.Caption = "Iteration name:"
            Me!Label1.BackColor = &H800000
            Me!Label2.BackColor = &H800000
        Case "Sequence  "
            frmItSeqBrSel.BackColor = &HC0&
            Me!Label1.Caption = "Sequence name:"
            Me!Label1.BackColor = &HC0&
            Me!Label2.BackColor = &HC0&
        Case "Branch    "
            frmItSeqBrSel.BackColor = &H4080&
            Me!Label1.Caption = "Branch name:"
            Me!Label1.BackColor = &H4080&
            Me!Label2.BackColor = &H4080&
        Case "Selection "
            frmItSeqBrSel.BackColor = &H8000&
            Me!Label1.Caption = "Selection name:"
            Me!Label1.BackColor = &H8000&
            Me!Label2.BackColor = &H8000&
    End Select
    
End Sub
