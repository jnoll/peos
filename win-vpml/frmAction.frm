VERSION 5.00
Begin VB.Form frmAction 
   BackColor       =   &H00000000&
   Caption         =   "Action info"
   ClientHeight    =   5670
   ClientLeft      =   60
   ClientTop       =   360
   ClientWidth     =   8925
   LinkTopic       =   "Form1"
   ScaleHeight     =   5670
   ScaleWidth      =   8925
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton CmdMvPrv 
      Caption         =   "Move Previous"
      Enabled         =   0   'False
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   4680
      TabIndex        =   6
      Top             =   3960
      Width           =   1815
   End
   Begin VB.CommandButton CmdMvNxt 
      Caption         =   "Move Next"
      Enabled         =   0   'False
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   6600
      TabIndex        =   7
      Top             =   3960
      Width           =   1815
   End
   Begin VB.CommandButton CmdDeleteSelector 
      Caption         =   "Delete"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   2760
      TabIndex        =   5
      Top             =   3960
      Width           =   1815
   End
   Begin VB.CommandButton CmdAddSelector 
      Caption         =   "Add Another"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   840
      TabIndex        =   4
      Top             =   3960
      Width           =   1815
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
      Left            =   600
      List            =   "frmAction.frx":001C
      Style           =   2  'Dropdown List
      TabIndex        =   2
      Top             =   3270
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
      ItemData        =   "frmAction.frx":0061
      Left            =   2880
      List            =   "frmAction.frx":006B
      Style           =   2  'Dropdown List
      TabIndex        =   1
      Top             =   1680
      Width           =   5040
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
      Height          =   390
      Left            =   3360
      TabIndex        =   3
      Top             =   3270
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
      Height          =   405
      Left            =   2880
      TabIndex        =   0
      Top             =   1080
      Width           =   5040
   End
   Begin VB.CommandButton cmdOkAddActions 
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
      Left            =   2400
      TabIndex        =   8
      Top             =   5040
      Width           =   1695
   End
   Begin VB.CommandButton cmdCancelForm 
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
      Left            =   4440
      TabIndex        =   9
      Top             =   5040
      Width           =   1695
   End
   Begin VB.Label Label6 
      Alignment       =   2  'Center
      BackColor       =   &H00FFFFFF&
      Caption         =   "Add Selectors"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   14.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   480
      TabIndex        =   15
      Top             =   2160
      Width           =   1935
   End
   Begin VB.Shape Shape1 
      BorderColor     =   &H00FFFFFF&
      Height          =   2535
      Left            =   360
      Top             =   2280
      Width           =   8295
   End
   Begin VB.Label Label5 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Selector"
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
      Left            =   3360
      TabIndex        =   14
      Top             =   2760
      Width           =   1215
   End
   Begin VB.Label Label4 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Specification Type"
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
      Left            =   600
      TabIndex        =   13
      Top             =   2760
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
      TabIndex        =   12
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
      TabIndex        =   11
      Top             =   1080
      Width           =   1815
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      BackColor       =   &H00FFFFFF&
      Caption         =   "Action Information"
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
      Left            =   1320
      TabIndex        =   10
      Top             =   240
      Width           =   5895
   End
End
Attribute VB_Name = "FrmAction"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmbspec_change()
    SelectorArray(ArrayCurrPos).Type = Me!cmbspec.Text
End Sub

Private Sub CmdAddSelector_Click()
    If ArrayCurrPos = ArraySizeVar And SelectorArray(ArrayCurrPos).Type = "" Then
        If LoadArray = False Then
            Exit Sub
        End If
    End If
    ArraySizeVar = ArraySizeVar + 1
    ReDim Preserve SelectorArray(ArraySizeVar) As Selector
    ArrayCurrPos = ArraySizeVar
    txtselector.Text = ""
    CmdMvPrv.Enabled = True
    CmdMvNxt.Enabled = False
    
End Sub

Private Sub CmdDeleteSelector_Click()
Dim x As Integer
    For x = ArrayCurrPos To ArraySizeVar - 1
        SelectorArray(x).Type = SelectorArray(x + 1).Type
        SelectorArray(x).Selector = SelectorArray(x + 1).Selector
    Next x
    SelectorArray(ArraySizeVar).Type = ""
    SelectorArray(ArraySizeVar).Selector = ""
    ReDim Preserve SelectorArray(ArraySizeVar - 1) As Selector
    ArraySizeVar = ArraySizeVar - 1
    If ArrayCurrPos = ArraySizeVar + 1 Then
        ArrayCurrPos = ArrayCurrPos - 1
    End If
    If ArrayCurrPos = ArraySizeVar Then
        CmdMvNxt.Enabled = False
    End If
    If ArrayCurrPos = 0 Then
        CmdMvPrv.Enabled = False
    End If
    If ArrayCurrPos >= 0 Then
        Me!cmbspec.Text = SelectorArray(ArrayCurrPos).Type
        Me!txtselector.Text = SelectorArray(ArrayCurrPos).Selector
    End If
    
End Sub

Private Sub CmdMvNxt_Click()
    If ArrayCurrPos = 0 Then
        CmdMvPrv.Enabled = True
    End If
    ArrayCurrPos = ArrayCurrPos + 1
    Me!cmbspec.Text = SelectorArray(ArrayCurrPos).Type
    Me!txtselector.Text = SelectorArray(ArrayCurrPos).Selector
    If ArrayCurrPos = ArraySizeVar Then
        CmdMvNxt.Enabled = False
    End If
End Sub

Private Sub CmdMvPrv_Click()
    If ArrayCurrPos = ArraySizeVar And SelectorArray(ArrayCurrPos).Type = "" Then
        If LoadArray = False Then
            Exit Sub
        End If
    End If
    If ArrayCurrPos = ArraySizeVar Then
        CmdMvNxt.Enabled = True
    End If
    ArrayCurrPos = ArrayCurrPos - 1
    Me!cmbspec.Text = SelectorArray(ArrayCurrPos).Type
    Me!txtselector.Text = SelectorArray(ArrayCurrPos).Selector
    If ArrayCurrPos = 0 Then
        CmdMvPrv.Enabled = False
    End If
End Sub

Private Sub cmdOkAddActions_Click()
Dim x As Integer
    If txtGlbProcessName = "" Then
        MsgBox "You must first put in the process name.", vbOKOnly
    Else
        Call LoadArray
        For x = 0 To ArraySizeVar
            With glbObjectInfoTable
                .AddNew
                !processname = txtGlbProcessName
                !objecttype = "action"
                !Name = Me!txtprocessname
                !actiontype = Me!cmbtype
                !spectype = SelectorArray(x).Type
                !Selector = SelectorArray(x).Selector
                !Apictsindex = glbIndex
                .Update
            End With
        Next x
    End If
    Unload Me

End Sub

Private Sub cmdCancelForm_Click()
    Unload Me
End Sub

Private Function LoadArray()
    ReDim Preserve SelectorArray(ArraySizeVar) As Selector
    If cmbspec.Text = "" Then
        MsgBox " A specification type is required"
        LoadArray = False
        Exit Function
    End If
    SelectorArray(ArrayCurrPos).Type = cmbspec.Text
    SelectorArray(ArrayCurrPos).Selector = txtselector.Text
    LoadArray = True
End Function

Private Sub Form_Activate()
    If txtGlbProcessName = "" Then
        MsgBox "You must first put in the process name.", vbOKOnly
        Call cmdCancelForm_Click
        Exit Sub
    End If
    Me!cmbtype.Text = "manual"
    
End Sub
Private Sub Form_Load()
    ArraySizeVar = 0
    ArrayCurrPos = 0
    ReDim Preserve SelectorArray(ArraySizeVar) As Selector
End Sub

Private Sub txtselector_LostFocus()
    SelectorArray(ArrayCurrPos).Selector = Me!txtselector.Text
End Sub
