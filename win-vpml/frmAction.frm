VERSION 5.00
Begin VB.Form frmAction 
   BackColor       =   &H00000000&
   Caption         =   "Action info"
   ClientHeight    =   6630
   ClientLeft      =   60
   ClientTop       =   360
   ClientWidth     =   8250
   LinkTopic       =   "Form1"
   ScaleHeight     =   6630
   ScaleWidth      =   8250
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton DelItemButton 
      Caption         =   "Delete Item"
      Height          =   375
      Left            =   6600
      TabIndex        =   13
      Top             =   4560
      Width           =   1095
   End
   Begin VB.CommandButton AddListButton 
      Caption         =   "Add Item"
      Height          =   375
      Left            =   6600
      TabIndex        =   12
      Top             =   4080
      Width           =   1095
   End
   Begin VB.ListBox SpecList 
      Height          =   1815
      Left            =   240
      TabIndex        =   11
      Top             =   3600
      Width           =   6135
   End
   Begin VB.ComboBox cmbspec 
      BackColor       =   &H00FFFFFF&
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   390
      ItemData        =   "frmAction.frx":0000
      Left            =   3000
      List            =   "frmAction.frx":0013
      Style           =   2  'Dropdown List
      TabIndex        =   2
      Top             =   2280
      Width           =   2655
   End
   Begin VB.ComboBox cmbtype 
      BackColor       =   &H00FFFFFF&
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   390
      ItemData        =   "frmAction.frx":0040
      Left            =   3000
      List            =   "frmAction.frx":0042
      Style           =   2  'Dropdown List
      TabIndex        =   1
      Top             =   1680
      Width           =   3495
   End
   Begin VB.TextBox txtselector 
      BackColor       =   &H00FFFFFF&
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   360
      Left            =   3000
      TabIndex        =   3
      Top             =   2880
      Width           =   4695
   End
   Begin VB.TextBox txtprocessname 
      BackColor       =   &H00FFFFFF&
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   360
      Left            =   3000
      TabIndex        =   0
      Top             =   1095
      Width           =   4695
   End
   Begin VB.CommandButton Command1 
      Caption         =   "OK"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   2040
      TabIndex        =   4
      Top             =   6000
      Width           =   1695
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Cancel"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   4320
      TabIndex        =   6
      Top             =   6000
      Width           =   1695
   End
   Begin VB.Label Label5 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Source/ Information"
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
      Left            =   240
      TabIndex        =   10
      Top             =   2865
      Width           =   2535
   End
   Begin VB.Label Label4 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Specification Type:"
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
      Left            =   240
      TabIndex        =   9
      Top             =   2340
      Width           =   2535
   End
   Begin VB.Label Label3 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Type:"
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
      Left            =   1920
      TabIndex        =   8
      Top             =   1680
      Width           =   855
   End
   Begin VB.Label Label1 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Action Name:"
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
      Left            =   960
      TabIndex        =   7
      Top             =   1080
      Width           =   1815
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      BackColor       =   &H00FFFFFF&
      Caption         =   "Input the following information for the Action then click the OK button."
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
      Left            =   1320
      TabIndex        =   5
      Top             =   120
      Width           =   5655
   End
End
Attribute VB_Name = "FrmAction"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub AddListButton_Click()

End Sub

Private Sub Command1_Click()
    If txtGlbProcessName = "" Then
        MsgBox "You must first put in the process name.", vbOKOnly
    Else
        With glbObjectInfoTable
            .AddNew
            !processname = txtGlbProcessName
            !objecttype = "action"
            !Name = Me!txtprocessname
            !actiontype = Me!cmbtype
            !spectype = Me!cmbspec
            !selector = Me!txtselector
            !Apictsindex = glbIndex
            .Update
        End With
    End If
    Unload Me

End Sub

Private Sub Command2_Click()
    Unload Me
End Sub

