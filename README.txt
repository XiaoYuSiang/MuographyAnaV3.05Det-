# MuographyAnaV3.05Det-
Public Muography Analysis Code for Detector V3.05
Muography Analysis Code for Detector V2.0
Public Muography Analysis Code for Detector V2.0, V3.00, V3.05
Muography Analysis Code for Detector V2.0, V3.00, V3.05

This is the analysis program of Muography for Detector V2.0 made by Yu Siang Xiao.
This is the analysis program of Muography for Detector V2.0~ V3.05 made by Yu Siang Xiao.

00.Download the "MainControl.C" , "AnaVariable.h" & "/Macro" onto the NAS.
00.Download the "MainControl.C" , "AnaVariable.h" , RenewMacros.h & "/Macro" onto the NAS.
Ps: If you wan use multiple cpu to analyze the different "run" data parallelly. You can download the Run.C to same dir, and set the parameter like the MainControl.C.

01.Revise the analysis variable in the "AnaVariable.h"

@@ -14,7 +15,8 @@ Now, You can change the information in the MainControl.C

path_Hk : raw house keeping datas (ex: XXX_Hk.txt) default: "/data4/YuSiang/ExampleTestSpace/Data/" (example)

03.The path you want to save the operation data and the result: path_OpR: default: "/data4/YuSiang/ExampleTestSpace/Ana/"
03.The path you want to save the operation data and the result: 
    path_OpR: default: "/data4/YuSiang/ExampleTestSpace/Ana/"

04.The path of the folder which put the analysis "program files" path_Mac:
default: ./Macro/

