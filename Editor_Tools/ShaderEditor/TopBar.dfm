object fraTopBar: TfraTopBar
  Left = 0
  Top = 0
  Width = 443
  Height = 18
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Align = alClient
  Constraints.MaxHeight = 18
  Constraints.MinHeight = 18
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBlack
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  ParentFont = False
  TabOrder = 0
  object paView: TPanel
    Left = 149
    Top = 0
    Width = 159
    Height = 18
    Align = alLeft
    BevelOuter = bvNone
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    object ebViewFront: TExtBtn
      Left = 0
      Top = 0
      Width = 15
      Height = 18
      Hint = 'Front View'
      Align = alNone
      HotTrack = True
      CloseButton = False
      Caption = 'F'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Margin = 2
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
      OnClick = ebViewClick
    end
    object ebViewLeft: TExtBtn
      Left = 30
      Top = 0
      Width = 15
      Height = 18
      Hint = 'Left View'
      Align = alNone
      HotTrack = True
      CloseButton = False
      Caption = 'L'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Margin = 2
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
      OnClick = ebViewClick
    end
    object ebViewTop: TExtBtn
      Left = 60
      Top = 0
      Width = 15
      Height = 18
      Hint = 'Top View'
      Align = alNone
      HotTrack = True
      CloseButton = False
      Caption = 'T'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Margin = 2
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
      OnClick = ebViewClick
    end
    object ebViewBack: TExtBtn
      Left = 15
      Top = 0
      Width = 15
      Height = 18
      Hint = 'Back View'
      Align = alNone
      HotTrack = True
      CloseButton = False
      Caption = 'B'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Margin = 2
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
      OnClick = ebViewClick
    end
    object ebViewRight: TExtBtn
      Left = 45
      Top = 0
      Width = 15
      Height = 18
      Hint = 'Right View'
      Align = alNone
      HotTrack = True
      CloseButton = False
      Caption = 'R'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Margin = 2
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
      OnClick = ebViewClick
    end
    object ebViewBottom: TExtBtn
      Left = 75
      Top = 0
      Width = 15
      Height = 18
      Hint = 'Bottom View'
      Align = alNone
      HotTrack = True
      CloseButton = False
      Caption = 'B'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Margin = 2
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
      OnClick = ebViewClick
    end
    object ebCameraPlane: TExtBtn
      Left = 91
      Top = 0
      Width = 20
      Height = 18
      Hint = 'Camera Style'
      Align = alNone
      HotTrack = True
      CloseButton = False
      GroupIndex = 1
      Down = True
      Caption = 'P'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Margin = 1
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
      OnClick = ebCameraClick
    end
    object ebCameraArcBall: TExtBtn
      Tag = 1
      Left = 111
      Top = 0
      Width = 20
      Height = 18
      Hint = 'Camera Style'
      Align = alNone
      HotTrack = True
      CloseButton = False
      GroupIndex = 1
      Caption = 'A'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Margin = 1
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
      OnClick = ebCameraClick
    end
    object ebCameraFly: TExtBtn
      Tag = 2
      Left = 131
      Top = 0
      Width = 20
      Height = 18
      Hint = 'Camera Style'
      Align = alNone
      HotTrack = True
      CloseButton = False
      GroupIndex = 1
      Caption = 'F'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Margin = 1
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
      OnClick = ebCameraClick
    end
  end
  object Panel1: TPanel
    Left = 127
    Top = 0
    Width = 22
    Height = 18
    Align = alLeft
    BevelOuter = bvNone
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    object ebZoomExtents: TExtBtn
      Left = 0
      Top = 0
      Width = 20
      Height = 18
      Hint = 'Zoom Extents'
      Align = alNone
      HotTrack = True
      CloseButton = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Glyph.Data = {
        36060000424D3606000000000000360000002800000020000000100000000100
        18000000000000060000120B0000120B0000000000000000000035356C292993
        3030981F1F86585860C1C1BEBDBCBDC2C8C7AEB5B4A6A7A7C5C5C252525F2020
        8A2F2F962626903F3F728A8A869191919393938B8B8B8F8F8FBCBCBCBBBBBBBF
        BFBFB6B6B6B1B1B1BEBEBE8D8D8D8C8C8C9393939090908D8D8B2525892323EC
        1818D63C3C7DBEC0B8C3C8C8B0B3B38C777A77373E35191CB1B2B3BBBBB43434
        7C1919D92020E62E2E8C8C8C86A3A3A39C9C9C8F8F8FBBBBBBBFBFBFB6B6B6A0
        A0A08D8D8D787878B6B6B6B8B8B88E8E8E9C9C9CA1A1A190908B2D2D901918D5
        4B4B75C0C8B8AAA9A9805F63733B43805357827273311D1E6B474BC3C9C8BCBC
        AA4242731717D83535928F8F899C9C9B929292BBBBBBB2B2B29898988D8D8D95
        95959D9D9D7878788F8F8FBFBFBFB7B7B78F8F8F9C9C9C93938E1B1B7F414283
        A8A997815B60753A42825C5F90848394908F9086853F34355F3F42807375CACD
        CEB9B9A833337B272786878784929292AFAFAF9898988D8D8D979797A4A4A4A8
        A8A8A5A5A58080808A8A8A9D9D9DC2C2C2B5B5B58E8E8E8C8C8A58585FB9BDB6
        84525A885F63918A8997908F9B9090A09393A093934D33357067675B4144A3A3
        A3C7C7C6B2B2AB5A5A638B8B8BBABAB99696969A9A9AA5A5A5A8A8A8A9A9A9AB
        ABABABABAB838383979797898989B0B0B0BFBFBFB5B5B58E8E8FC5C4C1B2B6B6
        834E569A91919C9090A29494A69898A99D9DACA09F5E383C6E656576696A6558
        58BDBFBFBDBDBEC2C2BFBEBEBEB7B7B7959595A9A9A9A9A9A9ABABABADADADB0
        B0B0B1B1B1888888959595989898909090BBBBBBBBBBBBBCBCBCBEBFBFA4A0A1
        8A555BAA9F9FA99B9BADA0A0B1A2A2B4A6A6B8ABAB713D416E6161847B7A6959
        59ADAEAEBDBDBDBABABABCBCBCAFAFAF989898B0B0B0AFAFAFB1B1B1B2B2B2B4
        B4B4B6B6B68D8D8D9494949F9F9F919191B4B4B4BBBBBBB9B9B9C1C3C3998B8E
        9D6A70B1ABA9B4A6A6B8A9AABBABABBFAFAEC6BEBD80454B6F6364877C7C6E5C
        5EA5A3A3BEBFBFB9B9B9BDBDBDA9A9A9A1A1A1B4B4B4B3B3B3B5B5B5B7B7B7B8
        B8B8BDBDBD929292969696A1A1A1939393AFAFAFBCBCBCB9B9B9C2C6C590787C
        A88084BEB2B1C1B0B0C7BCBBC6C1BFC1BAB8B8A3A5922F3B7467688B84837764
        64979293C0C1C1B8B8B8BEBEBEA1A1A1A9A9A9B8B8B8B9B9B9BDBDBDBEBEBEBB
        BBBBB4B4B4929292979797A3A3A3979797A9A9A9BDBDBDB9B8B8BFC6C5826568
        BBA7A8C3BCB9C2ADADBA9295B9747FC26574D67887EC939EB04754847E7D7C70
        6F8B8485C0C1C1BABABABEBEBE9A9A9AB6B6B6BCBCBCB8B8B8B1B1B1AAAAAAA8
        A8A8B1B1B1BDBDBD9E9E9EA0A0A09B9B9BA3A3A3BCBCBCBABABAC2C9C672484D
        C27882C96E7BDB7988EEA0ACFCCBD2FFE6E9FFF2F1FCF7F6F8BEC3C554637F6C
        6D807878C3C4C3C2C2C0BEBEBE909090ADADADACACACB3B3B3C0C0C0CCCCCCD4
        D4D4D6D6D6D7D7D7C9C9C9A5A5A59B9B9B9D9D9DBEBEBEBFBFBF5D61657F6561
        994651E8BDC1FFFFFEFFFDFBF9EBEAF5E2E2F1DEDEEDD9D9E8E1E0EEC2C6B646
        55785C5FBABFB75D5C648D8D8F989898989898C5C5C5DADADAD9D9D9D3D3D3D0
        D0D0CECECECDCDCDCDCDCDC7C7C79F9F9F969696B9B9B98F8F901C1C7E4A4E8C
        A8B09E857A7D926E71CAA6AAE6D1D1DDC7C8CBB4B5BAA0A2A4898A927E7F8D67
        6B8671633E4186272684888885969696B1B1B1A0A0A09F9F9FB9B9B9C9C9C9C5
        C5C5BCBCBCB4B4B4AAAAAAA3A3A39D9D9D9A9A9A9393938C8C8A2E2E941A1ADA
        4D4C73C8CBBBADAFB086777A8B6B6F8B76788B7C7E8F87889B9597AEADAEB7BD
        AB4A4F7B1717DB36369591918B9D9D9D919191BDBDBDB4B4B49F9F9F9D9D9DA0
        A0A0A1A1A1A5A5A5AAAAAAB5B5B5B6B6B69393939D9D9D94948F2A2A902929F7
        1D1DDE454584C2C2BAC1C4C4B4B7B7BABCBCBDC0C0BDBEBFC1C3C3BEBEB64141
        871F1FE12525F23131928E8E88A7A7A79F9F9F939393BCBCBCBDBDBDB8B8B8BA
        BABABCBCBCBCBCBCBEBEBEBABABA9393939F9F9FA5A5A592928D2D2D661F1F8F
        2424920B0B764F4F56C1C1BEBBBABAB8B8B8B8B8B8BABABBC0C0BD4949540C0C
        7B2323901D1D8C36366D8686838D8D8D8F8F8F8383838B8B8BBCBCBCBABABAB9
        B9B9B9B9B9BABABABCBCBC8989898585858E8E8E8C8C8C8B8B88}
      Margin = 1
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
      OnClick = ebZoomExtentsClick
    end
  end
  object paTBAction: TPanel
    Left = 0
    Top = 0
    Width = 127
    Height = 18
    Align = alLeft
    BevelOuter = bvNone
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    object ebActionMove: TExtBtn
      Left = 50
      Top = 0
      Width = 25
      Height = 18
      Hint = 'Move'
      Align = alNone
      HotTrack = True
      CloseButton = False
      GroupIndex = 1
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Glyph.Data = {
        36060000424D3606000000000000360000002800000020000000100000000100
        18000000000000060000120B0000120B00000000000000000000B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B8B8B8C0C0C0AAAAAAABABABC0C0C0B8B8B8B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BABABAB6
        B6B6B7B7B7BABABAB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9BBBBBBC7C9C9554F4F554D4DC5C7C7BBBBBBB9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BABABABCBCBCA4
        A4A4A4A4A4BCBCBCBABABAB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B7B7B7CACCCC767373686060877D7D797676CACACAB7B6B6B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BDBDBDACACACA8
        A8A8AEAEAEACACACBDBDBDB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B8B8B8C2C2C29D9D9D4B4040978E8EC8BEBE887E7E989898C2C2C2B8B8
        B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BBBBBBB4B4B4A2A2A2B2
        B2B2BCBCBCAFAFAFB3B3B3BBBBBBB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B7B7B7C0C1C1C9C8C87C79796C68687069697068685F5A5A787474CCCDCDC2C2
        C2B7B6B6B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BBBBBBBCBCBCADADADA9A9A9AA
        AAAAAAAAAAA7A7A7ACACACBEBEBEBBBBBBB9B9B9B9B9B9B9B9B9B8B8B8BABABA
        CCCDCDA3A3A3807C7CC5C6C6CACCCC6C65656C6565CCCECEC6C7C77C79799D9C
        9CCACCCCBABABAB8B8B8B9B9B9B9B9B9BDBDBDB5B5B5ADADADBCBCBCBDBDBDA9
        A9A9A9A9A9BDBDBDBCBCBCACACACB4B4B4BEBEBEBABABAB8B8B8C0BFBFCACBCB
        7A7777433939726C6CCACACAC9CBCB6E67676E6767C9CBCBCBCCCC6662626D63
        637B7878C9CACABFC0C0BABABABDBDBDACACACA1A1A1ABABABBCBCBCBDBDBDA9
        A9A9A9A9A9BDBDBDBDBDBDA9A9A9AAAAAAADADADBDBDBDBDBDBDAEAFAF524D4D
        4B41417369695850506E68686E6767574F4F574F4F6E67676F6868554D4D9A91
        91665C5C534B4BAEACACBBBBBBA4A4A4A2A2A2AAAAAAA4A4A4A9A9A9A9A9A9A5
        A5A5A5A5A5A9A9A9A9A9A9A4A4A4B2B2B2A8A8A8A5A5A5BCBCBCA7A7A7474141
        554A4A897F7F554D4D6F68686E6767574F4F574F4F6E67676E6868585151786F
        6F5348484D4646ACABABBABABAA2A2A2A4A4A4AEAEAEA4A4A4A9A9A9A9A9A9A5
        A5A5A5A5A5A9A9A9A9A9A9A5A5A5ABABABA4A4A4A3A3A3BBBBBBC1C1C1C2C3C3
        6D6A6A524A4A656060CBCCCCC9CBCB6E67676E6767C9CBCBC9CACA7571713F34
        346E6D6DC7C8C8C0C0C0BABABABBBBBBABABABA4A4A4A8A8A8BDBDBDBDBDBDA9
        A9A9A9A9A9BDBDBDBCBCBCABABABA0A0A0AAAAAABCBCBCBFBFBFB8B8B8BDBDBD
        C9CACA8F8E8E747070C7C8C8CACCCC6D66666C6666C8C9C9C6C6C67B7979999A
        9ACBCCCCBBBBBBB8B8B8B9B9B9BABABABDBDBDB1B1B1ABABABBCBCBCBDBDBDA9
        A9A9A9A9A9BCBCBCBCBCBCACACACB3B3B3BDBDBDBABABAB9B8B8B9B9B9B9B9B9
        B7B7B7C4C4C4C6C7C77874746F6969635D5D696161827E7E858181C6C6C6C2C3
        C3B7B7B7B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BCBCBCBCBCBCABABABAAAAAAA7
        A7A7A9A9A9AEAEAEAFAFAFBCBCBCBBBBBBB9B9B9B9B9B9B8B8B8B9B9B9B9B9B9
        B9B9B9B8B8B8C3C3C3929393514848857E7E7369694439399E9D9DC2C2C2B8B8
        B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BCBCBCB3B3B3A4A4A4AE
        AEAEABABABA1A1A1B4B4B4BBBBBBB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B8B8B8C9CACA6C6A6A514848493F3F747171CDCECEB6B6B6B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BDBDBDAAAAAAA3
        A3A3A2A2A2ABABABBEBEBEB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9BDBDBDC2C4C44842424C4646C9CACABBBBBBB9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BABABABBBBBBA3
        A3A3A3A3A3BCBCBCBABABAB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B8B8B8C1C2C2A7A7A7ABABABC0C1C1B8B8B8B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BBBBBBB6
        B6B6B6B6B6BABABAB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9}
      Margin = 3
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
    end
    object ebActionRotate: TExtBtn
      Left = 75
      Top = 0
      Width = 25
      Height = 18
      Hint = 'Rotate'
      Align = alNone
      HotTrack = True
      CloseButton = False
      GroupIndex = 1
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Glyph.Data = {
        36060000424D3606000000000000360000002800000020000000100000000100
        18000000000000060000120B0000120B00000000000000000000B9B9B9B9B9B9
        B9B9B9B7B7B7BCBCBCC9CACABEBFBFB3B4B4B0B1B1B5B6B6C1C2C2BFBFBFB8B7
        B7B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BABABABDBDBDBABABAB8
        B8B8B8B8B8B9B9B9BBBBBBBBBBBBB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B8B8B8C7C8C8B8B9B97875755952525C55556F65657D7676848080A4A4A4C3C4
        C4B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BDBDBDB9B9B9ADADADA6A6A6A5
        A5A5A9A9A9ACACACAFAFAFB5B5B5BCBCBCB9B9B9B9B9B9B9B9B9B9B9B9B7B7B7
        CACBCB9493933F37374B41416D6363827A7AA59B9BCCC3C3CFC5C59A91918B88
        88C1C2C2BABABAB9B9B9B9B9B9B9B9B9BDBDBDB1B1B1A0A0A0A2A2A2A9A9A9AE
        AEAEB4B4B4BDBDBDBDBDBDB2B2B2B0B0B0BBBBBBBABABAB9B9B9B7B7B7C7C8C8
        9797973A30305E55555C51512F2A2A242222282626443F3FBCB7B7F7EFEF9C92
        92888686C5C7C7B7B7B7B9B9B9BCBCBCB3B3B39F9F9FA6A6A6A5A5A59D9D9D9B
        9B9B9C9C9CA1A1A1BABABAC5C5C5B2B2B2B0B0B0BCBCBCB9B9B9BDBDBDB7B7B7
        483E3E675E5E4D45452321217A7B7B9FA0A09B9B9B6767671615157B7373D5CC
        CC736A6AA5A5A5C0C1C1B9B9B9BABABAA2A2A2A8A8A8A3A3A39B9B9BADADADB5
        B5B5B4B4B4A9A9A9989898ACACACBEBEBEAAAAAAB6B6B6BBBBBBC7C8C8858080
        655C5C746B6B232121ADAFAFD2D2D2C1C1C1C1C1C1CDCDCDA0A1A11A18188F87
        87877D7D706C6CC6C7C7BCBCBCAFAFAFA8A8A8ABABAB9B9B9BB7B7B7BFBFBFBB
        BBBBBBBBBBBEBEBEB4B4B49A9A9AB1B1B1AEAEAEABABABBCBCBCBCBEBE6F6A6A
        9F9595474242797A7AD3D4D4B5B5B5B8B8B8B8B8B8B8B8B8CECECE797A7A3830
        307D7575544C4CB9B9B9BFBFBFAAAAAAB3B3B3A1A1A1ADADADBFBFBFB9B9B9B9
        B9B9B9B9B9B9B9B9BEBEBEACACAC9E9E9EACACACA4A4A4B9B9B9AFAFAF7E7878
        C1B8B83D3939ADAFAFBEBFBFBDBDBDB8B8B8B9B9B9B8B8B8C0C0C0A9AAAA2F2A
        2A675E5E484040AEAFAFBDBEBEACACACBABABAA0A0A0B8B8B8BBBBBBBABABAB9
        B9B9B9B9B9B9B9B9BABABAB7B7B79D9D9DA8A8A8A2A2A2B7B7B7ADAEAE8B8383
        DBD2D2423F3FAEAFAFC5C5C5B3B3B3BCBCBCB9B9B9B8B8B8BEBEBEB1B2B2302B
        2B584F4F403A3AB0AFAFBEBDBDAFAFAFC0C0C0A1A1A1B7B7B7BCBCBCB8B8B8BA
        BABAB9B9B9B9B9B9BABABAB8B8B89D9D9DA5A5A5A0A0A0B7B7B7B5B6B67E7878
        DDD3D3665F5F7C7D7D767373676464CCCDCDB8B8B8B7B7B7CAC8C8979898322A
        2A504747433D3DBCBDBDBEBEBEADADADC0C0C0A8A8A8ADADADACACACA9A9A9BD
        BDBDB9B9B9B9B9B9BDBDBDB3B3B39E9E9EA3A3A3A1A1A1BABABAC3C4C47F7B7B
        A49A9AAAA2A2312D2D7D75754F4B4BC8CBCBB9B9B9BDBDBDB8B9B93934344941
        413D3535656262CACCCCBBBBBBAEAEAEB4B4B4B6B6B69D9D9DACACACA3A3A3BD
        BDBDB9B9B9BABABAB9B9B99F9F9FA2A2A2A0A0A0A9A9A9BDBDBDC0BFBFA6A7A7
        6C64649489898B8181837C7C3C3838BABABABDBDBDBEBEBEB2B2B24C4949342A
        2A322A2AAFAEAEBFC0C0BABABAB6B6B6A9A9A9B1B1B1AFAFAFADADAD9F9F9FBA
        BABABABABABABABAB8B8B8A3A3A39E9E9E9E9E9EB7B7B7BBBBBBB8B8B8CFD2D2
        5F5A5A6159596A6161625959362E2EA0A0A0C2C2C2B8B8B8C0BFBFB6B8B84D4C
        4C7B7B7BCBCBCBB6B6B6BABABABFBFBFA7A7A7A7A7A7A9A9A9A7A7A79E9E9EB4
        B4B4BBBBBBB9B9B9BBBBBBB9B9B9A3A3A3ADADADBDBDBDB9B9B9C5C5C56F6B6B
        3228284D42424F4646534949342C2C7E7D7DC7C8C8B8B8B8B8B8B8BDBDBDC1C2
        C2C4C5C5B8B8B8B9B9B9BDBDBDABABAB9D9D9DA2A2A2A3A3A3A4A4A49D9D9DAD
        ADADBCBCBCB9B9B9B9B9B9BBBBBBBCBCBCBCBCBCB9B9B9B9B9B9BCBCBCA3A1A1
        8D8C8C7370705451514641412A1F1F4B4848CDCECEB6B6B6B9B9B9B9B9B9BBBB
        BBB9B9B9B9B9B9B9B9B9BABABAB5B5B5B0B0B0ABABABA5A5A5A2A2A29B9B9BA3
        A3A3BEBEBEB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BFBFBF
        C6C7C7C6C8C8C3C5C5B7B7B79A9A9A979696C0C0C0B8B8B8B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9BBBBBBBCBCBCBDBDBDBCBCBCBABABAB3B3B3B2
        B2B2BBBBBBB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9}
      Margin = 3
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
    end
    object ebActionScale: TExtBtn
      Left = 100
      Top = 0
      Width = 25
      Height = 18
      Hint = 'Uniform Scale'
      Align = alNone
      HotTrack = True
      CloseButton = False
      GroupIndex = 1
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Glyph.Data = {
        36060000424D3606000000000000360000002800000020000000100000000100
        18000000000000060000120B0000120B00000000000000000000B9B9B9B9B9B9
        B9B9B9B8B8B9B8B8B9B8B8B9B8B8B9B8B8B9B8B8B9B8B8B8B8B8B8B8B8B8B8B8
        B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B8B8B9
        BDBDBCC9C9BCC8C8BBC8C8BBC8C8BBC9C9BBC9C8BCC6C6C5C6C6C7C6C6C6C8C8
        C8C0C0C0B8B8B8B9B9B9B9B9B9B9B9B9BBBBBBBEBEBEBDBDBDBDBDBDBDBDBDBD
        BDBDBDBDBDBFBFBFBFBFBFBFBFBFC0C0C0BCBCBCB9B9B9B9B9B9B9B9B9BEBEBC
        AEADAB75769A6F72A27073A16F71A06E6FA07576A08786868585848685857E7E
        7EA3A3A3C0C0C0B9B9B9B9B9B9BBBBBBB3B3B3A3A3A3A4A4A4A4A4A4A3A3A3A3
        A3A3A5A5A5A3A3A3A1A1A1A1A1A19E9E9EAFAFAFBCBCBCB9B9B9B8B8B9C7C7BE
        818299423DCC9C90DE9087DD9086E1998DE26B64E93A3467584E41514747352C
        2C717171C9C9C9B8B8B8B9B9B9BEBEBEA6A6A6A2A2A2B9B9B9B6B6B6B6B6B6B8
        B8B8B2B2B28A8A8A8888888888887C7C7C999999C1C1C1B9B9B9B8B8B8C6C6BD
        7D809D8880D8FFFFEAFFF9EBFFFCECFFFFECBBB9FA4C47857970626F6566544B
        4B7B7B7BC7C7C7B8B8B8B9B9B9BDBDBDA6A6A6B3B3B3D8D8D8D6D6D6D6D6D6D9
        D9D9C8C8C89595959797979696968A8A8A9E9E9EC0C0C0B9B9B9B9B9B9C5C5BD
        7E809C847CD7FFFEF5FFF1F4FFF5F7FFFFF8B2B2FE524B897F756872696A564D
        4D7B7A7AC7C7C7B8B8B8B9B9B9BDBDBDA6A6A6B2B2B2D8D8D8D7D7D7D7D7D7DA
        DADAC7C7C79898989A9A9A9797978A8A8A9D9D9DC0C0C0B8B8B8B9B9B9C5C5BD
        7E7F9B857ED9FFFFFFFFFEFEFFFFFFFFFFFFB2B3FE5A5490867D6E7770715B52
        527C7C7CC7C7C7B8B8B8B9B9B9BDBDBDA5A5A5B3B3B3DBDBDBD9D9D9D9D9D9DC
        DCDCC7C7C79B9B9B9D9D9D9A9A9A8D8D8D9E9E9EC0C0C0B8B8B8BABABAC6C6BD
        7B7B9B8B89DCFFFFFFFFFFFFFFFFFFFFFFFFB9BAFF5E57948B82747B72735D54
        547B7B7BC6C7C7B8B8B8B9B9B9BDBDBDA4A4A4B6B6B6DFDFDFDBDBDBDBDBDBDF
        DFDFC9C9C99C9C9C9F9F9F9B9B9B8E8E8E9D9D9DC0C0C0B8B8B8BABABAC6C6BD
        84859E5C5EDFBCBCFDB1B2FBB0B1FBB7B7FB8081FC6A639A8D83777D74755E54
        547B7A7AC6C7C7B8B8B8B9B9B9BDBDBDA7A7A7ADADADCACACAC6C6C6C6C6C6C8
        C8C8BCBCBCA1A1A1A0A0A09C9C9C8E8E8E9D9D9DC0C0C0BCBCBCB9B9B9C4C4C3
        90918E645B7E776E9E79729F7A73A0756D9E776F9A908787867D7B7C74745D54
        547B7B7BC6C7C7B8B8B8B9B9B9BEBEBEA6A6A6999999A4A4A4A5A5A5A5A5A5A4
        A4A4A3A3A3A5A5A5A0A0A09B9B9B8E8E8E9D9D9DC0C0C0B8B8B8B8B8B8C3C3C3
        8B8C8B7D7469A1978DA0978DA39A8EA0978B9B91858D83818079797A71715C53
        537B7C7CC6C7C7B8B8B8B9B9B9BEBEBEA5A5A59A9A9AAAAAAAAAAAAAABABABAA
        AAAAA7A7A7A3A3A39E9E9E9B9B9B8E8E8E9D9D9DC0C0C0B8B8B8B8B8B8C3C4C4
        8C8D8D7A70709B91919A91919A9191978E8F9187878980807F76767970705A52
        527C7B7BC7C7C7B8B8B8B9B9B9BEBEBEA6A6A69A9A9AAAAAAAA9A9A9A9A9A9A8
        A8A8A5A5A5A2A2A29D9D9D9A9A9A8C8C8C9E9E9EC0C0C0B9B9B9B8B8B8C6C5C5
        8688885A51517C72727A7070797070756D6D7268686960606058585B51513C32
        32727272C8C9C9B8B8B8B9B9B9BFBFBFA3A3A38C8C8C9C9C9C9A9A9A9B9B9B99
        99999797979494948F8F8F8D8D8D7F7F7F999999C1C1C1B9B9B9B9B9B9BEBEBE
        ABABAB8080808486868586868586868686868686868686868686868686867F7F
        7FA3A3A3C0C0C0B9B9B9B9B9B9BBBBBBB3B3B39F9F9FA2A2A2A2A2A2A2A2A2A2
        A2A2A2A2A2A2A2A2A2A2A2A3A3A39F9F9FAFAFAFBCBCBCB9B9B9B9B9B9B8B8B8
        BEBEBEC6C6C6C3C4C4C3C4C4C3C4C4C4C4C4C4C4C4C4C5C5C5C5C5C5C5C5C7C7
        C7BFC0C0B8B8B8B9B9B9B9B9B9B9B9B9BBBBBBBFBFBFBEBEBEBEBEBEBEBEBEBE
        BEBEBEBEBEBEBEBEBEBEBEBFBFBFBFBFBFBCBCBCB9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8
        B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9}
      Margin = 3
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
    end
    object ebActionSelect: TExtBtn
      Left = 0
      Top = 0
      Width = 25
      Height = 18
      Hint = 'Select'
      Align = alNone
      HotTrack = True
      CloseButton = False
      GroupIndex = 1
      Down = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Glyph.Data = {
        36060000424D3606000000000000360000002800000020000000100000000100
        18000000000000060000120B0000120B00000000000000000000B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BDBDBDA0A0A0757575AAAAAABEBEBEB9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BB
        BBBBAEAEAE9B9B9BB3B3B3BCBCBCB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B8B8B8BBBBBBBFBFBF535353727272989898959595C1C1
        C1B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BABABABC
        BCBC8B8B8B999999ABABABA9A9A9BDBDBDB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9BEBEBEC3C3C3BCBCBCA9A9A92A2A2AC7C7C7888888B4B4B4BCBC
        BCB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BCBCBCBEBEBEBABABAB2
        B2B2797979C0C0C0A4A4A4B7B7B7BBBBBBB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9BDBDBDA1A1A1979797CCCCCC848484474747F3F3F36D6D6DCBCBCBB8B8
        B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BBBBBBAFAFAFAAAAAAC2C2C2A2
        A2A2868686D3D3D3989898C2C2C2B8B8B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9C5C5C58484842626266262622E2E2E979797B3B3B3AEAEAEC4C4C4B7B7
        B7B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BFBFBFA2A2A27777779292927B
        7B7BAAAAAAB7B7B7B5B5B5BEBEBEB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B8B8B8C5C5C58181815D5D5DBEBEBE7C7C7CE5E5E56A6A6A757575ADADADBEBE
        BEB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BFBFBFA0A0A0909090BBBBBB9D
        9D9DCDCDCD9696969A9A9AB4B4B4BCBCBCB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9C4C4C4808080525252BBBBBBB2B2B2FAFAFAF8F8F8CECECE6262629797
        97C2C2C2B8B8B8B9B9B9B9B9B9B9B9B9B9B9B9BFBFBFA0A0A08A8A8ABABABAB7
        B7B7D7D7D7D5D5D5C6C6C6919191AAAAAABEBEBEB9B9B9B8B8B8B9B9B9B9B9B9
        B9B9B9C4C4C47F7F7F555555B8B8B8B9B9B9FFFFFFFFFFFFCCCCCC6F6F6FC4C4
        C4B8B8B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BFBFBF9F9F9F8C8C8CB9B9B9BA
        BABAD9D9D9DEDEDEC5C5C5989898BEBEBEB9B9B9B9B9B9BCBDBDB9B9B9B9B9B9
        B9B9B9C4C4C47F7F7F565656B5B5B5D9D9D9FFFFFFE8E8E8606060C0C0C0BDBD
        BDB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BFBFBF9F9F9F8D8D8DB7B7B7C8
        C8C8DCDCDCCFCFCF919191BDBDBDBCBCBCB9B9B9B9B9B9BDBDBDB9B9B9B9B9B9
        B9B9B9C4C4C47F7F7F555555BBBBBBF1F1F1FFFFFF777777B0B0B0BFBFBFB8B8
        B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BFBFBF9F9F9F8D8D8DBBBBBBD3
        D3D3DDDDDD9C9C9CB5B5B5BCBCBCB8B8B8B9B9B9B9B9B9B8B8B8B9B9B9B9B9B9
        B9B9B9C4C4C47E7E7E525252CFCFCFFFFFFF878787969696C7C7C7B7B7B7B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BFBFBF9F9F9F8B8B8BC3C3C3DF
        DFDFA3A3A3AAAAAAC0C0C0B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9C4C4C47F7F7F4D4D4DF8F8F8A5A5A57B7B7BCCCCCCB7B7B7B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BFBFBF9F9F9F898989D7D7D7B0
        B0B09D9D9DC2C2C2B8B8B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9C5C5C5797979676767C3C3C3616161CCCCCCB7B7B7B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BFBFBF9C9C9C959595BEBEBE92
        9292C3C3C3B8B8B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B8B8B8C4C4C47979795D5D5D6A6A6AC3C3C3BCBCBCB9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BEBEBE9D9D9D919191969696BE
        BEBEBBBBBBB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9C0C0C0A0A0A0303030C4C4C4BEBEBEB8B8B8B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BDBDBDAFAFAF7C7C7CBEBEBEBC
        BCBCB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9BABABAB8B8B8B5B5B5BFBFBFB8B8B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BABABAB9B9B9B7B7B7BCBCBCB8
        B8B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9}
      Margin = 3
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
    end
    object ebActionAdd: TExtBtn
      Left = 25
      Top = 0
      Width = 25
      Height = 18
      Hint = 'Add'
      Align = alNone
      HotTrack = True
      CloseButton = False
      GroupIndex = 1
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Glyph.Data = {
        36060000424D3606000000000000360000002800000020000000100000000100
        18000000000000060000120B0000120B00000000000000000000B9B9B9B9B9B9
        B8B8B8BCBCBCB6B6B68788885050503837373130303232324444446F6F6FA8A8
        A8BCBCBCB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BBBBBBB8B8B8A3A3A38A8A8A80
        80807C7C7C7C7C7C858585989898B2B2B2BBBBBBB9B9B9B9B9B9B9B9B9B8B8B8
        C4C4C4C4C4C45D5E5E151414221D1D3A3434464040413A3A2B26261310104040
        40AFB0B0CBCBCBBCBCBCB9B9B9B9B9B9BEBEBEBFBFBF9090906F6F6F7474747F
        7F7F8383838181817979796E6E6E838383B5B5B5C1C1C1BABABABCBCBCC7C7C7
        A3A3A36665651A1818494242736A6A887D7D948B8B8A8181756C6C5A52521E19
        19575757969696AFAFAFBCBCBCC0C0C0B0B0B0949494717171858585979797A1
        A1A1A6A6A6A2A2A29999998D8D8D7272728D8D8DA9A9A9B5B5B5B3B3B38F8F8F
        3C3C3C1415153C36366B61617D7373958A8AA49A9A978E8E7F75756F6666463E
        3E2420203637379C9C9CBBBBBBA7A7A78181816F6F6F7F7F7F9494949C9C9CA7
        A7A7ADADADA8A8A89D9D9D9595958484847575757F7F7FACACACA8A8A8A4A4A4
        7474742220204742426D63638279799B9090ACA0A09F9494857A7A7269694039
        39848383544C4CB0B0B0B8B8B8B0B0B09A9A9A7575758585859595959F9F9FA9
        A9A9B1B1B1ABABAB9F9F9F979797818181A1A1A18A8A8AB6B6B6BCBCBCCDCECE
        5C59597F7474342F2F6C64648278789D9191B0A3A3A19595847B7B6D6464413C
        3CD5D6D6767676555454C1C1C1C3C3C38F8F8F9D9D9D7D7D7D9494949F9F9FAA
        AAAAB2B2B2ADADAD9F9F9F959595828282C6C6C69B9B9B8B8B8BBDBDBDB2B3B3
        3D35358F8B8B7D7A7A4F4545887C7CA59A9ABAADADABA0A0897E7E645A5A433F
        3F9A9999585656686666C1C1C1B7B7B7808080A5A5A59E9E9E878787A1A1A1AE
        AEAEB8B8B8B0B0B0A1A1A1919191838383ABABAB8D8D8D959595D0D1D17E7C7C
        413939C1C3C3BCBCBC544D4D675B5B716767817777746A6A6A5E5E564F4F8683
        838985859A9696C3C3C3C5C5C59E9E9E828282BEBEBEBBBBBB8A8A8A91919197
        97979E9E9E9898989494948B8B8BA2A2A2A3A3A3AAAAAABEBEBE868686524A4A
        A3A1A1C2C3C3BDBDBDBCBCBCACABAB9E9D9D312B2B908E8EAFB0B0B7B7B7C8C9
        C9C3C4C4C2C3C3BCBCBCA2A2A2898989AFAFAFBEBEBEBBBBBBBBBBBBB3B3B3AD
        ADAD7A7A7AA6A6A6B5B5B5B9B9B9C1C1C1BEBEBEBEBEBEBABABAB1B0B0C1C0C0
        C0C1C1B8B8B8B8B8B8BABABAC2C3C3B7B8B8969292B1B1B1C2C4C4BBBBBBB7B7
        B7B9B8B8B9B9B9B9B9B9B5B5B5BCBCBCBDBDBDB9B9B9B9B9B9BABABABDBDBDB9
        B9B9A9A9A9B6B6B6BEBEBEBABABAB8B8B8B9B9B9B9B9B9B9B9B9BDBDBDBBBBBB
        B9B9B9B9B9B9B9B9B9B9B9B9B8B8B8BABABAC4C4C4BBBBBBB8B8B8B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9BBBBBBBABABAB9B9B9B9B9B9B9B9B9B9B9B9B9B9B9BA
        BABABEBEBEBABABAB8B8B8B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9
        B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9B9}
      Margin = 3
      NumGlyphs = 2
      ParentFont = False
      Spacing = 15
      Transparent = False
    end
  end
  object fsStorage: TFormStorage
    IniSection = 'Top Bar'
    Options = []
    RegistryRoot = prLocalMachine
    OnRestorePlacement = fsStorageRestorePlacement
    StoredProps.Strings = (
      'ebCameraArcBall.Down'
      'ebCameraFly.Down'
      'ebCameraPlane.Down')
    StoredValues = <>
    Left = 65522
    Top = 65520
  end
end
