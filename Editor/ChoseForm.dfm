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
          F5FFFFFF07000000E0AF0A0700100000FFFFFFFF0000010101006C6178000000
          00000000102700000001006E64897706000000000000010E0000000000000104
          00000000000000000001000000000000C40F0000CB00000002000000A3000000
          01000000CB00000002000000A300000001000000CB00000002000000A3000000
          01000000B2000000AC000000BC000000AC000000010000000001000000000000
          000000000000E0AF0A0700100000FFFFFFFF0000010101006C61780000000000
          0000102700000001006EBC083404010000000000010E00000000000001040000
          0000000000000001000000000000C40F0000CB00000002000000A30000000100
          0000CB00000002000000A300000001000000CB00000002000000A30000000100
          0000B2000000AC000000BC000000AC0000000100000000010000000000000000
          00000000E0AF0A0700100000FFFFFFFF0000010101006C617800000000000000
          102700000001006EDC3D2604020000000000010E000000000000010400000000
          000000000001000000000000C40F0000CB00000002000000A300000001000000
          CB00000002000000A300000001000000CB00000002000000A300000001000000
          B2000000AC000000BC000000AC00000001000000000100000000000000000000
          0000E0AF0A0700100000FFFFFFFF0000010101006C6178000000000000001027
          00000001006E4022300D030000000000010E0000000000000104000000000000
          00000001000000000000C40F0000CB00000002000000A300000001000000CB00
          000002000000A300000001000000CB00000002000000A300000001000000B200
          0000AC000000BC000000AC000000010000000001000000000000000000000000
          E0AF0A0700100000FFFFFFFF0000010101006C61780000000000000010270000
          0001006EFCCF2A04040000000000010E00000000000001040000000000000000
          0001000000000000C40F0000CB00000002000000A300000001000000CB000000
          02000000A300000001000000CB00000002000000A300000001000000B2000000
          AC000000BC000000AC000000010000000001000000000000000000000000E0AF
          0A0700100000FFFFFFFF0000010101006C617800000000000000102700000001
          006E1C0E440D050000000000010E000000000000010400000000000000000001
          000000000000C40F0000CB00000002000000A300000001000000CB0000000200
          0000A300000001000000CB00000002000000A300000001000000B2000000AC00
          0000BC000000AC000000010000000001000000000000000000000000E0AF0A07
          00100000FFFFFFFF0000010101006C617800000000000000102700000001006E
          28886A00060000000000010E0000000000000104000000000000000000010000
          00000000C40F0000CB00000002000000A300000001000000CB00000002000000
          A300000001000000CB00000002000000A300000001000000B2000000AC000000
          BC000000AC000000010000000001000000000000000000000000}
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
      F5FFFFFF07000000A0EFFF0D00100000FFFFFFFF0000010101003A0E78000000
      00000000102700000001006E206F2C0400000000000065200000000000000103
      00000000000000000001000000000000D00F0000200000001700000000000000
      050000007365464F340000001F000000840004521CA5E80600000000E9090000
      EF0900002097E90694B6060400BF060420000000010000000001000000000000
      000000000000A0EFFF0D00100000FFFFFFFF0000010101003A0E780000000000
      0000102700000001006E18452904010000000000652000000000000001030000
      0000000000000001000000000000D00F00002000000017000000000000000500
      00007365464F340000001F000000840004521CA5E80600000000E9090000EF09
      00002097E90694B6060400BF0604200000000100000000010000000000000000
      00000000A0EFFF0D00100000FFFFFFFF0000010101003A0E7800000000000000
      102700000001006EAC2976060200000000006520000000000000010300000000
      000000000001000000000000D00F000020000000170000000000000005000000
      7365464F340000001F000000840004521CA5E80600000000E9090000EF090000
      2097E90694B6060400BF06042000000001000000000100000000000000000000
      0000A0EFFF0D00100000FFFFFFFF0000010101003A0E78000000000000001027
      00000001006ED8CF160C03000000000065200000000000000103000000000000
      00000001000000000000D00F0000200000001700000000000000050000007365
      464F340000001F000000840004521CA5E80600000000E9090000EF0900002097
      E90694B6060400BF060420000000010000000001000000000000000000000000
      A0EFFF0D00100000FFFFFFFF0000010101003A0E780000000000000010270000
      0001006EF0A92304040000000000652000000000000001030000000000000000
      0001000000000000D00F0000200000001700000000000000050000007365464F
      340000001F000000840004521CA5E80600000000E9090000EF0900002097E906
      94B6060400BF060420000000010000000001000000000000000000000000A0EF
      FF0D00100000FFFFFFFF0000010101003A0E7800000000000000102700000001
      006E106126040500000000006520000000000000010300000000000000000001
      000000000000D00F0000200000001700000000000000050000007365464F3400
      00001F000000840004521CA5E80600000000E9090000EF0900002097E90694B6
      060400BF060420000000010000000001000000000000000000000000A0EFFF0D
      00100000FFFFFFFF0000010101003A0E7800000000000000102700000001006E
      1065720006000000000065200000000000000103000000000000000000010000
      00000000D00F0000200000001700000000000000050000007365464F34000000
      1F000000840004521CA5E80600000000E9090000EF0900002097E90694B60604
      00BF060420000000010000000001000000000000000000000000}
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
    OnItemFocused = tvItemsItemFocused
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
