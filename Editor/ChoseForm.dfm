object frmChoseItem: TfrmChoseItem
  Left = 579
  Top = 333
  BorderStyle = bsDialog
  Caption = 'Select Item'
  ClientHeight = 331
  ClientWidth = 346
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 208
    Top = 0
    Width = 138
    Height = 331
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 0
    object sbSelect: TExtBtn
      Left = 2
      Top = 312
      Width = 67
      Height = 18
      Align = alNone
      BevelShow = False
      CloseButton = False
      Caption = 'Ok'
      Transparent = False
      FlatAlwaysEdge = True
      OnClick = sbSelectClick
    end
    object sbCancel: TExtBtn
      Left = 70
      Top = 312
      Width = 67
      Height = 18
      Align = alNone
      BevelShow = False
      CloseButton = False
      Caption = 'Cancel'
      Transparent = False
      FlatAlwaysEdge = True
      OnClick = sbCancelClick
    end
    object lbItemName: TMxLabel
      Left = 33
      Top = 141
      Width = 106
      Height = 13
      AutoSize = False
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ShadowColor = 15263976
    end
    object lbFileName: TMxLabel
      Left = 33
      Top = 155
      Width = 106
      Height = 13
      AutoSize = False
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ShadowColor = 15263976
    end
    object mxLabel1: TMxLabel
      Left = 1
      Top = 141
      Width = 32
      Height = 13
      AutoSize = False
      Caption = 'Item:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      ShadowColor = 15263976
    end
    object mxLabel2: TMxLabel
      Left = 1
      Top = 155
      Width = 28
      Height = 13
      AutoSize = False
      Caption = 'File:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      ShadowColor = 15263976
    end
    object mxLabel3: TMxLabel
      Left = 1
      Top = 168
      Width = 28
      Height = 13
      AutoSize = False
      Caption = 'Info:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      ShadowColor = 15263976
    end
    object lbInfo: TMxLabel
      Left = 33
      Top = 168
      Width = 106
      Height = 13
      AutoSize = False
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ShadowColor = 15263976
      WordWrap = True
    end
    object paImage: TPanel
      Left = 3
      Top = 6
      Width = 132
      Height = 132
      BevelOuter = bvLowered
      Caption = '<no image>'
      TabOrder = 0
      object pbImage: TPaintBox
        Left = 1
        Top = 1
        Width = 130
        Height = 130
        Align = alClient
        Color = clGray
        ParentColor = False
        OnPaint = pbImagePaint
      end
    end
    object paMulti: TPanel
      Left = 1
      Top = 226
      Width = 136
      Height = 83
      TabOrder = 1
      Visible = False
      object ebMultiUp: TExtBtn
        Left = 120
        Top = 3
        Width = 14
        Height = 18
        Align = alNone
        CloseButton = False
        Glyph.Data = {
          F6000000424DF600000000000000360000002800000008000000080000000100
          180000000000C0000000120B0000120B00000000000000000000FFFFFFFFFFFF
          FFFFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000
          00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDDDDDC000000000000DDDDDCFFFFFFFF
          FFFFFFFFFF000000000000000000000000191919000000FFFFFFFFFFFFDDDDDC
          000000000000000000000000DDDDDCFFFFFFFFFFFFFFFFFF0000000000000000
          00000000FFFFFFFFFFFFFFFFFFFFFFFFDDDDDC000000000000DDDDDCFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFF000000000000FFFFFFFFFFFFFFFFFF}
        Spacing = 0
        Transparent = False
        OnClick = ebMultiUpClick
      end
      object ebMultiDown: TExtBtn
        Left = 120
        Top = 63
        Width = 14
        Height = 18
        Align = alNone
        CloseButton = False
        Glyph.Data = {
          F6000000424DF600000000000000360000002800000008000000080000000100
          180000000000C0000000120B0000120B00000000000000000000FFFFFFFFFFFF
          FFFFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDDDDDC0000000000
          00DDDDDCFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000000FFFFFFFF
          FFFFFFFFFFDDDDDC000000000000000000000000DDDDDCFFFFFFFFFFFF000000
          191919000000000000000000000000FFFFFFFFFFFFFFFFFFDDDDDC0000000000
          00DDDDDCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000FFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFF000000000000FFFFFFFFFFFFFFFFFF}
        Spacing = 0
        Transparent = False
        OnClick = ebMultiDownClick
      end
      object ebMultiRemove: TExtBtn
        Left = 120
        Top = 23
        Width = 14
        Height = 18
        Align = alNone
        CloseButton = False
        Glyph.Data = {
          56010000424D560100000000000036000000280000000A000000090000000100
          18000000000020010000120B0000120B00000000000000000000C8D0D4C8D0D4
          C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D40000C8D0D4000000
          000000C8D0D4C8D0D4C8D0D4C8D0D4000000000000C8D0D40000C8D0D4C8D0D4
          000000000000C8D0D4C8D0D4000000000000C8D0D4C8D0D40000C8D0D4C8D0D4
          C8D0D4000000000000000000000000C8D0D4C8D0D4C8D0D40000C8D0D4C8D0D4
          C8D0D4C8D0D4000000000000C8D0D4C8D0D4C8D0D4C8D0D40000C8D0D4C8D0D4
          C8D0D4000000000000000000000000C8D0D4C8D0D4C8D0D40000C8D0D4C8D0D4
          000000000000C8D0D4C8D0D4000000000000C8D0D4C8D0D40000C8D0D4000000
          000000C8D0D4C8D0D4C8D0D4C8D0D4000000000000C8D0D40000C8D0D4C8D0D4
          C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D40000}
        Spacing = 0
        Transparent = False
        OnClick = ebMultiRemoveClick
      end
      object ebMultiClear: TExtBtn
        Left = 120
        Top = 43
        Width = 14
        Height = 18
        Align = alNone
        CloseButton = False
        Glyph.Data = {
          C2010000424DC20100000000000036000000280000000C0000000B0000000100
          1800000000008C010000120B0000120B00000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000000000FF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000FFFFFFFFFFFF000000000000
          FFFFFFFFFFFFFFFFFF000000000000FFFFFFFFFFFF0000000000000000000000
          00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000FFFFFFFFFFFF00000000
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000
          000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000
          00000000FFFFFFFFFFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00
          0000000000000000000000FFFFFFFFFFFF000000000000FFFFFFFFFFFFFFFFFF
          000000000000FFFFFFFFFFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000000000FFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFF}
        Spacing = 0
        Transparent = False
        OnClick = ebMultiClearClick
      end
      object tvMulti: TElTree
        Left = 1
        Top = 1
        Width = 119
        Height = 81
        Cursor = crDefault
        LeftPosition = 0
        DoubleBuffered = False
        DragCursor = crDrag
        AutoCollapse = False
        AutoExpand = True
        DockOrientation = doNoOrient
        DefaultSectionWidth = 120
        BorderSides = [ebsLeft, ebsRight, ebsTop, ebsBottom]
        DragAllowed = True
        ExplorerEditMode = False
        Flat = True
        FocusedSelectColor = 10526880
        FocusedSelectTextColor = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        FullRowSelect = False
        GradientSteps = 64
        HeaderHeight = 19
        HeaderHotTrack = False
        HeaderSections.Data = {
          F5FFFFFF070000000000BB06002A0000FFFFFFFF000001010100222E78000000
          00000000102700000001003E6489770600000000000020220000000000000120
          000000000000000000010000000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D010000000001000000000000
          0000000000000000BB06002A0000FFFFFFFF000001010100222E780000000000
          0000102700000001003EBC083404010000000000202200000000000001200000
          00000000000000010000000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D0100000000010000000000000000
          000000000000BB06002A0000FFFFFFFF000001010100222E7800000000000000
          102700000001003EDC3D26040200000000002022000000000000012000000000
          0000000000010000000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D01000000000100000000000000000000
          00000000BB06002A0000FFFFFFFF000001010100222E78000000000000001027
          00000001003E4022300D03000000000020220000000000000120000000000000
          000000010000000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D2D2D2D2D010000000001000000000000000000000000
          0000BB06002A0000FFFFFFFF000001010100222E780000000000000010270000
          0001003EFCCF2A04040000000000202200000000000001200000000000000000
          00010000000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D2D2D0100000000010000000000000000000000000000
          BB06002A0000FFFFFFFF000001010100222E7800000000000000102700000001
          003E1C0E440D0500000000002022000000000000012000000000000000000001
          0000000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D0100000000010000000000000000000000000000BB06
          002A0000FFFFFFFF000001010100222E7800000000000000102700000001003E
          28886A0006000000000020220000000000000120000000000000000000010000
          000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
          2D2D2D2D2D2D2D2D010000000001000000000000000000000000}
        HeaderFont.Charset = DEFAULT_CHARSET
        HeaderFont.Color = clWindowText
        HeaderFont.Height = -11
        HeaderFont.Name = 'MS Sans Serif'
        HeaderFont.Style = []
        HorzDivLinesColor = 7368816
        HorzScrollBarStyles.ShowTrackHint = False
        HorzScrollBarStyles.Width = 16
        HorzScrollBarStyles.ButtonSize = 14
        HorzScrollBarStyles.UseSystemMetrics = False
        IgnoreEnabled = False
        IncrementalSearch = False
        KeepSelectionWithinLevel = False
        LineBorderActiveColor = clBlack
        LineBorderInactiveColor = clBlack
        LineHeight = 17
        LinesColor = clBtnShadow
        MouseFrameSelect = True
        MultiSelect = False
        OwnerDrawMask = '~~@~~'
        PlusMinusTransparent = True
        ScrollbarOpposite = False
        ScrollTracking = True
        ShowButtons = False
        ShowLeafButton = False
        ShowLines = False
        StoragePath = '\Tree'
        TabOrder = 0
        TabStop = True
        TrackColor = 10526880
        VertDivLinesColor = 7368816
        VertScrollBarStyles.ShowTrackHint = True
        VertScrollBarStyles.Width = 16
        VertScrollBarStyles.ButtonSize = 14
        VertScrollBarStyles.UseSystemMetrics = False
        VirtualityLevel = vlNone
        TextColor = clBtnText
        BkColor = clGray
        OnDragDrop = tvMultiDragDrop
        OnDragOver = tvMultiDragOver
        OnStartDrag = tvMultiStartDrag
      end
    end
  end
  object tvItems: TElTree
    Left = 0
    Top = 0
    Width = 208
    Height = 331
    Cursor = crDefault
    LeftPosition = 0
    DoubleBuffered = False
    DragCursor = crDrag
    Align = alClient
    AutoCollapse = False
    AutoExpand = True
    DockOrientation = doNoOrient
    DefaultSectionWidth = 120
    BorderSides = [ebsLeft, ebsRight, ebsTop, ebsBottom]
    Color = clWhite
    CustomPlusMinus = True
    DrawFocusRect = False
    ExplorerEditMode = False
    Flat = True
    FocusedSelectColor = 10526880
    FocusedSelectTextColor = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    FullRowSelect = False
    GradientSteps = 64
    HeaderColor = 7368816
    HeaderHeight = 19
    HeaderHotTrack = False
    HeaderSections.Data = {
      F5FFFFFF070000000000BB06002A0000FFFFFFFF000001010100650078000000
      00000000102700000001003E206F2C0400000000000020220000000000000120
      000000000000000000010000000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D010000000001000000000000
      0000000000000000BB06002A0000FFFFFFFF0000010101006500780000000000
      0000102700000001003E18452904010000000000202200000000000001200000
      00000000000000010000000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D0100000000010000000000000000
      000000000000BB06002A0000FFFFFFFF00000101010065007800000000000000
      102700000001003EAC2976060200000000002022000000000000012000000000
      0000000000010000000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D01000000000100000000000000000000
      00000000BB06002A0000FFFFFFFF000001010100650078000000000000001027
      00000001003ED8CF160C03000000000020220000000000000120000000000000
      000000010000000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D2D2D2D2D010000000001000000000000000000000000
      0000BB06002A0000FFFFFFFF0000010101006500780000000000000010270000
      0001003EF0A92304040000000000202200000000000001200000000000000000
      00010000000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D2D2D0100000000010000000000000000000000000000
      BB06002A0000FFFFFFFF00000101010065007800000000000000102700000001
      003E106126040500000000002022000000000000012000000000000000000001
      0000000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D0100000000010000000000000000000000000000BB06
      002A0000FFFFFFFF00000101010065007800000000000000102700000001003E
      1065720006000000000020220000000000000120000000000000000000010000
      000000002D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
      2D2D2D2D2D2D2D2D010000000001000000000000000000000000}
    HeaderFont.Charset = DEFAULT_CHARSET
    HeaderFont.Color = clWindowText
    HeaderFont.Height = -11
    HeaderFont.Name = 'MS Sans Serif'
    HeaderFont.Style = []
    HorizontalLines = True
    HorzDivLinesColor = 7368816
    HorzScrollBarStyles.ShowTrackHint = False
    HorzScrollBarStyles.Width = 16
    HorzScrollBarStyles.ButtonSize = 14
    HorzScrollBarStyles.UseSystemMetrics = False
    HorzScrollBarStyles.UseXPThemes = False
    IgnoreEnabled = False
    IncrementalSearch = False
    KeepSelectionWithinLevel = False
    LineBorderActiveColor = clBlack
    LineBorderInactiveColor = clBlack
    LineHeight = 18
    LinesColor = clBtnShadow
    MinusPicture.Data = {
      F6000000424DF600000000000000360000002800000008000000080000000100
      180000000000C0000000120B0000120B00000000000000000000808080808080
      808080808080808080808080808080808080808080808080808080E0E0E08080
      80808080808080808080808080808080808080E0E0E080808080808080808080
      8080808080808080E0E0E0E0E0E0E0E0E0808080808080808080808080808080
      E0E0E0E0E0E0E0E0E0808080808080808080808080E0E0E0E0E0E0E0E0E0E0E0
      E0E0E0E0808080808080808080E0E0E0E0E0E0E0E0E0E0E0E0E0E0E080808080
      8080808080808080808080808080808080808080808080808080}
    MouseFrameSelect = True
    MultiSelect = False
    OwnerDrawMask = '~~@~~'
    PlusMinusTransparent = True
    PlusPicture.Data = {
      F6000000424DF600000000000000360000002800000008000000080000000100
      180000000000C0000000120B0000120B00000000000000000000808080808080
      8080808080808080808080808080808080808080808080808080808080808080
      80808080808080808080808080E0E0E0E0E0E080808080808080808080808080
      8080808080E0E0E0E0E0E0E0E0E0E0E0E0808080808080808080808080E0E0E0
      E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0808080808080E0E0E0E0E0E0E0E0E0E0E0
      E0808080808080808080808080E0E0E0E0E0E080808080808080808080808080
      8080808080808080808080808080808080808080808080808080}
    ScrollbarOpposite = False
    ScrollTracking = True
    ShowLeafButton = False
    ShowLines = False
    SortMode = smAdd
    StoragePath = '\Tree'
    TabOrder = 1
    TabStop = True
    Tracking = False
    TrackColor = 10526880
    VertDivLinesColor = 7368816
    VerticalLines = True
    VertScrollBarStyles.ShowTrackHint = True
    VertScrollBarStyles.Width = 16
    VertScrollBarStyles.ButtonSize = 14
    VertScrollBarStyles.UseSystemMetrics = False
    VertScrollBarStyles.UseXPThemes = False
    VirtualityLevel = vlNone
    UseXPThemes = False
    TextColor = clBtnText
    BkColor = clGray
    OnItemChange = tvItemsItemChange
    OnItemSelectedChange = tvItemsItemSelectedChange
    OnDblClick = tvItemsDblClick
    OnKeyPress = tvItemsKeyPress
  end
  object fsStorage: TFormStorage
    StoredValues = <
      item
        Name = 'EmitterDirX'
        Value = 0s
      end
      item
        Name = 'EmitterDirY'
        Value = 0s
      end
      item
        Name = 'EmitterDirZ'
        Value = 0s
      end>
    Left = 8
    Top = 8
  end
end
