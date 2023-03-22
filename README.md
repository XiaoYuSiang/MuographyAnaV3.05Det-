# Introduction of MuographyAnaV3.05Det-
Public Muography Analysis Code for Detector V2.0, V3.00, V3.05.
<br>
<br>This is the analysis program of Muography for Detector V2.0~ V3.05 made by Yu Siang Xiao.
<br>
Q: How to use? 
<br>
<br>00. Download the "MainControl.C" , "AnaVariable.h" , RenewMacros.h & "/Macro" onto the NAS.
<br>Ps: If you wan use multiple cpu to analyze the different "run" data parallelly. 
<br>    * You can download the Run.C to same dir, and set the parameter like the MainControl.C.
<br>
01. Revise the analysis variable in the "AnaVariable.h". 
* example: change the trigger for anylize, color palette, default fitting function parameter
+ detector/ scintillator size and numbers, runs time range.
<br>
<br>02. Now, You can change the information in the "MainControl.C". Find the marker @AdjustPath by searching in the file.
<br>
<br>03. Changing the path of floder which "input files" and want to do analysis for: 
<br>    03(0). Ori_path_Mac: raw(original) macros dir path example: "./Macro/"
<br>    03(1). path_Raw: raw datas (XXX_Mu.txt) example: "/data4/YuSiang/ExampleSpace/"
<br>    03(2). path_Hk : raw house keeping datas (XXX_Hk.txt) example: "/data4/YuSiang/ExampleSpace/HKData/"
<br>
<br>04. The path you want to save the root file of raw data (XXX_Mu.root), it can share with another runs data: 
<br>    path_Rot: example: "/data4/YuSiang/ExampleSpace/rootfile/"
<br> 
<br>    The path you want to save the operation data and the results: 
<br>    path_OpR: example: "/data4/YuSiang/ExampleSpace/Ana/"
<br>
<br>05. The path of the folder which put the analysis "program files" for this analysis:
<br>    path_Mac: default: "./MacroTmp/"
<br>
<br>06.Change the Data name want to be researched!! 
<br>    06(0). name_Raw: "*_Mu" find out the raw data: *_Mu.txt
<br>    06(1). name_Hk : "*_HK" find out the house keeping data: *_Mu.txt
<br>    PS: Examples of how to search are put in "MainControl.C"
<br>        A directory contain 3 files:    Name1.txt & Name.txt & N.txt
<br>        Ex1. case for name_Raw = "Name":
<br>            Will analyze the file: Name.txt
<br>        Ex2. case for name_Raw = "Name*":
<br>            Will analyze the file: Name.txt & Name1.txt
<br>        Ex3. case for name_Raw = "*":
<br>            Will analyze the file: Name.txt & Name1.txt % N.txt
<br>
<br>07.Change the operation mode for you want, and search marker @AdjustOpMode to adjust.
<br>    07(0).testMode        : default: false; //Test code by using small sample number in some pwidth ana. program.
<br>    07(1).normalizeMode   : default: true ; //Normalize the difference by the statistic in pwidth ana. program.
<br>    07(2).NegHiRatChMode  : default: true ; //Could not use now!
<br>    07(3).lowstatisticMode: default: false; //when the data come from large theta, use MuEelID to analyze the pwidth.
<br>    07(4).rootFileMode    : default: true ; //re save the data of pwidth ana. want to use. 
<br>    07(5).SETightMode     : default: true ; //select event tightly for MuEelID in pwidth ana. 
<br>    07(6).PointTextMode   : default: true ; //draw the data point of HK VS Rate to be text(save time).
<br>
<br>08.Set if you want to skip some block of program: 
<br>    08(0).bool SkipBlock  : default: true ; 
<br>        true : Skip the programs in the range for: if(SkipBlock==false){} 
<br>        false: Enable all of the programs except the "DSLAna.h"
<br>    08(1).bool ReDSLAna   : default: true ; 
<br>        rerun the DSLAna.h to save DSLData.h when you change anything in "AnaVariable.h" or change the select data.
<br>
<br>09.Select the program want to be used: Put the program block to the outer of range for: if(SkipBlock==false){}
<br>
<br>10.Go to the path you save the result, and check the result.
<br>
<br>11.If the program or the graph has something wrong, or the graph for result has something that needs to improve. Please connect with Yu Siang Xiao.
<br>
<br>Thank you for your use.
<br>
# Revise the issue regarding the inconvenient use of the 'BDINV[]' array in 'AnaVariable.h' by replacing its function with 'int BDCheck(int b)' from 'GobelFunctions.h'.
