object ItemList: TItemList
  Left = 593
  Top = 304
  Width = 259
  Height = 329
  BiDiMode = bdRightToLeft
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Item list'
  Color = clGray
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  ParentBiDiMode = False
  Scaled = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 0
    Top = 0
    Width = 251
    Height = 1
    Align = alTop
    Shape = bsFrame
  end
  object tvItems: TElTree
    Left = 0
    Top = 1
    Width = 251
    Height = 301
    Cursor = crDefault
    LeftPosition = 0
    HeaderPopupMenu = pmSystem
    DragCursor = crDrag
    Align = alClient
    AutoCollapse = False
    AutoLineHeight = False
    DockOrientation = doNoOrient
    DefaultSectionWidth = 120
    AdjustMultilineHeight = False
    BorderStyle = bsNone
    BorderSides = []
    CheckBoxGlyph.Data = {
      D6050000424DD60500000000000036000000280000003C000000080000000100
      180000000000A005000000000000000000000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF535353
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF535353FFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000FFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFF535353535353535353FFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFF000000000000000000FFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF535353535353535353FFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FF000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF53
      5353535353535353535353535353FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000
      000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5353535353
      53535353535353535353FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      535353FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000FFFFFFFFFF
      FF000000000000FFFFFFFFFFFFFFFFFF535353535353535353FFFFFFFFFFFF53
      5353535353FFFFFFFFFFFFFFFFFF000000000000000000FFFFFFFFFFFF000000
      000000FFFFFFFFFFFFFFFFFF535353535353535353FFFFFFFFFFFF5353535353
      53FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000FFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF535353535353535353FFFFFFFFFFFF
      FFFFFFFFFFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000000000FFFF
      FFFFFFFF535353535353FFFFFFFFFFFFFFFFFFFFFFFF535353535353FFFFFFFF
      FFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000000000FFFFFFFFFFFF
      535353535353FFFFFFFFFFFFFFFFFFFFFFFF535353535353FFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFF535353FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000FFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFF535353535353FFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFF535353535353FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFF535353535353FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      535353535353FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF535353FFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF535353}
    Color = clWhite
    CustomCheckboxes = True
    CustomPlusMinus = True
    DragAllowed = True
    DrawFocusRect = False
    DragTrgDrawMode = dtdDownColorLine
    ExplorerEditMode = True
    FilteredVisibility = True
    FocusedSelectColor = 10526880
    FocusedSelectTextColor = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    FullRowSelect = False
    GradientSteps = 64
    HeaderColor = 7368816
    HeaderHeight = 17
    HeaderHotTrack = False
    HeaderSections.Data = {
      F4FFFFFF01000000B451660400000000FFFFFFFF0000010100000000C3000000
      0000000010270000000101004014FF040000000001005B00FFFFFFFF00000100
      00000000000000000001000000000000D5E35B00010000000000000003000000
      0000000024EC5B0034EC5B004CEC5B005CEC5B006CEC5B0098EC5B008905B200
      9905B200B105B2006CA4CD0C3852660490000000060000004974656D73000100
      0000000000000000000000}
    HeaderFlat = True
    HeaderFont.Charset = DEFAULT_CHARSET
    HeaderFont.Color = clWindowText
    HeaderFont.Height = -11
    HeaderFont.Name = 'MS Sans Serif'
    HeaderFont.Style = []
    HorizontalLines = True
    HorzDivLinesColor = 7368816
    HorzScrollBarStyles.ShowTrackHint = False
    HorzScrollBarStyles.Width = 16
    HorzScrollBarStyles.ButtonSize = 16
    HorzScrollBarStyles.UseSystemMetrics = False
    HorzScrollBarStyles.UseXPThemes = False
    IgnoreEnabled = False
    IncrementalSearch = False
    InplaceEditorDelay = 0
    ItemIndent = 14
    KeepSelectionWithinLevel = False
    LineBorderActiveColor = clBlack
    LineBorderInactiveColor = clBlack
    LineHeight = 17
    LinesColor = clBtnShadow
    LinesStyle = psSolid
    MinusPicture.Data = {
      F6000000424DF600000000000000360000002800000008000000080000000100
      180000000000C000000000000000000000000000000000000000808080808080
      808080808080808080808080808080808080808080808080808080E0E0E08080
      80808080808080808080808080808080808080E0E0E080808080808080808080
      8080808080808080E0E0E0E0E0E0E0E0E0808080808080808080808080808080
      E0E0E0E0E0E0E0E0E0808080808080808080808080E0E0E0E0E0E0E0E0E0E0E0
      E0E0E0E0808080808080808080E0E0E0E0E0E0E0E0E0E0E0E0E0E0E080808080
      8080808080808080808080808080808080808080808080808080}
    MouseFrameSelect = True
    OwnerDrawMask = '~~@~~'
    ParentFont = False
    ParentShowHint = False
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
    QuickEditMode = True
    ScrollbarOpposite = False
    ShowColumns = True
    ShowCheckboxes = True
    ShowHint = False
    ShowLeafButton = False
    ShowLines = False
    StoragePath = '\Tree'
    SortUseCase = False
    StripedOddColor = clGray
    StripedEvenColor = 8158332
    StripedItems = True
    TabOrder = 0
    TabStop = True
    Tracking = False
    TrackColor = 10526880
    VertDivLinesColor = 7368816
    VertScrollBarStyles.ShowTrackHint = True
    VertScrollBarStyles.Width = 16
    VertScrollBarStyles.ButtonSize = 16
    VertScrollBarStyles.UseSystemMetrics = False
    VertScrollBarStyles.UseXPThemes = False
    VirtualityLevel = vlNone
    UseXPThemes = False
    TextColor = clBtnText
    BkColor = clGray
    OnAfterSelectionChange = tvItemsAfterSelectionChange
    OnItemChange = tvItemsItemChange
    OnItemDraw = tvItemsItemDraw
    OnResize = tvItemsResize
    OnClick = tvItemsClick
    OnDragDrop = tvItemsDragDrop
    OnMouseDown = tvItemsMouseDown
    OnMouseMove = tvItemsMouseMove
    OnMouseUp = tvItemsMouseUp
    OnKeyDown = tvItemsKeyDown
  end
  object fsStorage: TFormStorage
    StoredValues = <>
    Left = 65525
    Top = 65521
  end
  object pmSystem: TMxPopupMenu
    Alignment = paCenter
    MarginStartColor = 10921638
    MarginEndColor = 2763306
    BKColor = 10528425
    SelColor = clBlack
    SelFontColor = 10526880
    SepHColor = 1644825
    SepLColor = 13158600
    LeftMargin = 10
    Style = msOwnerDraw
    Top = 24
    object N3: TMenuItem
      Caption = '-'
    end
    object RefreshForm1: TMenuItem
      Caption = 'Refresh Form'
      OnClick = RefreshForm1Click
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object ExpandAll1: TMenuItem
      Caption = 'Expand All'
      OnClick = ExpandAll1Click
    end
    object CollapseAll1: TMenuItem
      Caption = 'Collapse All'
      OnClick = CollapseAll1Click
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object miDrawThumbnails: TMenuItem
      AutoCheck = True
      Caption = 'Draw Thumbnails'
      OnClick = miDrawThumbnailsClick
    end
  end
  object pmItems: TMxPopupMenu
    Alignment = paCenter
    MarginStartColor = 10921638
    MarginEndColor = 2763306
    BKColor = 10528425
    SelColor = clBlack
    SelFontColor = 10526880
    SepHColor = 1644825
    SepLColor = 13158600
    LeftMargin = 10
    Style = msOwnerDraw
    Top = 56
  end
  object InplaceEdit: TElTreeInplaceAdvancedEdit
    Tree = tvItems
    Types = [sftText]
    OnAfterOperation = InplaceEditAfterOperation
    OnValidateResult = InplaceEditValidateResult
    Top = 88
  end
  object pmEdit: TMxPopupMenu
    Alignment = paCenter
    MarginStartColor = 10921638
    MarginEndColor = 2763306
    BKColor = 10528425
    SelColor = clBlack
    SelFontColor = 10526880
    SepHColor = 1644825
    SepLColor = 13158600
    LeftMargin = 10
    Style = msOwnerDraw
    Left = 32
    Top = 56
    object MenuItem1: TMenuItem
      Caption = '-'
    end
    object MenuItem2: TMenuItem
      Caption = 'Create Folder'
      OnClick = miCreateFolderClick
    end
    object MenuItem3: TMenuItem
      Caption = '-'
    end
    object MenuItem4: TMenuItem
      Caption = 'Rename'
      OnClick = Rename1Click
    end
    object MenuItem5: TMenuItem
      Caption = 'Delete'
      OnClick = Delete1Click
    end
  end
end
