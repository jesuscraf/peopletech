�Cn�  �  �  ��           J$A    rI6A    J$A    rI6A    J$A �>V�y&A        3  0      �  0  <  0                  �   �          D  H  L                              a   i                2  P  e  H  j  �  �  $     V  �       �  5  �  :  �  �               �  T  	       �  b  h              T  �  3  0  �  0                                                                                                                              8                  0          �  �          �                              (                  	     �  �  �    �  L  �  �  �  �  �    �  \  �  �    �                 (     0	        8  �  �  P  H    1                                                  P  I   1  �	                                         `  `  P  J   1                                             �  �  P  K   1                                                �  P  L   1                                             �  ,  P  M   1                                             �  p  P  N   1                                             @  �  P  O   1                                             �  �  P  P   1                                             P  P               �       5  3  0  5  0                                                                                                                              �                  0          l  �          �                              0                      �  �  �  �  k    p  `                 �     p        �  8  �  H  Q    1                                             �  �  H  R   1  �	                                         �  0  H  S   1                                             X  t  H  T   1                                             H  H               �        �  B  3  0  B  0                                                                                                                                                  0          	                                              8               	     u  8	  x  �	    
  �  L
  �  �
  �  �
  �    �  \  �  �               �  �        L	  �  U    1                                            �	  �	  �	  �  8	  �     �  8	         8	  h     h  �	  �  V   1                                             �  
  �  W   1                                             (  `
  �  X   1                                             �  �
  �  Y   1                                             �  �
  �  Z   1                                             H  ,  �  [   1                                             �  p  �  \   1                                               �  �  ]   1                                             �  P         �  H         �  $         �  �         h  �  3  0  �  0                                                                                                                              D                  0          @  T          \                              @                       h  �  �  �  �                 4     ����      D  �  |  $  ^    1  �	                                            �  $  _   1                                             `    $  `   1                                             $  $               �    �  �      0  0  �      	   �     `  �     �  �  @  �  P               �  1  1  0  1           1  1  1  1  1  1                                 E  �  1  1  0  1           1  1  1  1  1  1                                   �  1  1  0  1           1  1  1  1  1  1                                 )  �  1  1  0  1           1  1  1  1  1  1                                 A  �  1  1  0  1           1  1  1  1  1  1                                 Y  �  1  1  0  1           1  1  1  1  1  1                                 q  �  1  1  0  1           1  1  1  1  1  1                                 �  �  1  1  0  1           1  1  1  1  1  1                                 �  �  1  1  0  1           1  1  1  1  1  1                         5  �      0  0  0         8  �  �  X  H               5  1  1  0  1           1  1  1  1  1  1                                 E  5  1  1  0  1           1  1  1  1  1  1                                 �  5  1  1  0  1           1  1  1  1  1  1                                 �  5  1  1  0  1           1  1  1  1  1  1                         B  �      0  0  �      	     h  �  (  �  �  H  �    �                 E  B  1  1  0  1           1  1  1  1  1  1                                 P  B  1  1  0  1           1  1  1  1  1  1                                 _  B  1  1  0  1           1  1  1  1  1  1                                 w  B  1  1  0  1           1  1  1  1  1  1                                 �  B  1  1  0  1           1  1  1  1  1  1                                 �  B  1  1  0  1           1  1  1  1  1  1                                 �  B  1  1  0  1           1  1  1  1  1  1                                 �  B  1  1  0  1           1  1  1  1  1  1                                 �  B  1  1  0  1           1  1  1  1  1  1                         �        0  0  �         �     `  $                 E  �  1  1  0  1           1  1  1  1  1  1                                 �  �  1  1  0  1           1  1  1  1  1  1                                 �  �  1  1  0  1           1  1  1  1  1  1                         �  0    1  1  1      5  0  L  1  1  1      B  0  Z  1  1  1      �  0  �  1  1  1        M4TRANS OLTP SYS_DICT_LOGIC_OBJECTS  SELECT @ID_OBJECT = ID_OBJECT, @LANG_2_ID_TRANS_OBJENG = ID_TRANS_OBJENG, @LANG_3_ID_TRANS_OBJESP = ID_TRANS_OBJESP, @LANG_4_ID_TRANS_OBJFRA = ID_TRANS_OBJFRA, @LANG_5_ID_TRANS_OBJGER = ID_TRANS_OBJGER, @LANG_6_ID_TRANS_OBJBRA = ID_TRANS_OBJBRA, @LANG_7_ID_TRANS_OBJITA = ID_TRANS_OBJITA, @LANG_8_ID_TRANS_OBJGEN = ID_TRANS_OBJGEN FROM M4RDC_LOGIC_OBJECT WHERE ID_OBJECT = @ID_OBJECT_AUX AND DTE_CLOSED = PLUS_INF  SYS_DICT_FIELDS  SELECT @ID_FIELD = ID_FIELD, @LANG_2_ID_TRANS_FLDENG = ID_TRANS_FLDENG, @LANG_3_ID_TRANS_FLDESP = ID_TRANS_FLDESP, @LANG_4_ID_TRANS_FLDFRA = ID_TRANS_FLDFRA, @LANG_5_ID_TRANS_FLDGER = ID_TRANS_FLDGER, @LANG_6_ID_TRANS_FLDBRA = ID_TRANS_FLDBRA, @LANG_7_ID_TRANS_FLDITA = ID_TRANS_FLDITA, @LANG_8_ID_TRANS_FLDGEN = ID_TRANS_FLDGEN FROM M4RDC_FIELDS WHERE ID_OBJECT = @ID_OBJECT AND DTE_CLOSED = PLUS_INF SYS_DICT_LOGIC_OBJECTS_TRANS wSELECT @ID_TRANS_OBJ = ID_TRANS_OBJ#, @ID_LANGUAGE = ID_LANGUAGE# FROM M4RDC_LOGIC_OBJECT# WHERE ID_OBJECT = @ID_OBJECT SYS_DICT_FIELDS_TRANS �SELECT @ID_FIELD = T.ID_FIELD, @ID_TRANS_FLD = T.ID_TRANS_FLD#, @ID_LANGUAGE = T.ID_LANGUAGE# FROM M4RDC_FIELDS# T, M4RDC_FIELDS F WHERE F.ID_OBJECT = T.ID_OBJECT AND T.ID_FIELD = F.ID_FIELD AND F.ID_OBJECT = @ID_OBJECT AND F.DTE_CLOSED = PLUS_INF 	ID_OBJECT ID_OBJECT_AUX LANG_2_ID_TRANS_OBJENG LANG_3_ID_TRANS_OBJESP LANG_4_ID_TRANS_OBJFRA LANG_5_ID_TRANS_OBJGER LANG_6_ID_TRANS_OBJBRA LANG_7_ID_TRANS_OBJITA LANG_8_ID_TRANS_OBJGEN ID_FIELD LANG_2_ID_TRANS_FLDENG LANG_3_ID_TRANS_FLDESP LANG_4_ID_TRANS_FLDFRA LANG_5_ID_TRANS_FLDGER LANG_6_ID_TRANS_FLDBRA LANG_7_ID_TRANS_FLDITA LANG_8_ID_TRANS_FLDGEN ID_TRANS_OBJ ID_LANGUAGE ID_TRANS_FLD 