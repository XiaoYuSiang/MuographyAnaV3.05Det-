Public Muography Analysis Code for Detector V2.0, V3.00, V3.05.

This is the analysis program of Muography for Detector V2.0~ V3.05 made by Yu Siang Xiao.

Q: How to use? 

00. Download the "MainControl.C" , "AnaVariable.h" , RenewMacros.h & "/Macro" onto the NAS.
    Ps: If you wan use multiple cpu to analyze the different "run" data parallelly. 
        You can download the Run.C to same dir, and set the parameter like the MainControl.C.

01. Revise the analysis variable in the "AnaVariable.h". 
    example: change the trigger for anylize, color palette, default fitting function parameter
             detector/ scintillator size and numbers, runs time range.

02. Now, You can change the information in the "MainControl.C". Find the marker @AdjustPath by searching in the file.

03. Changing the path of floder which "input files" and want to do analysis for: 
    03(0). Ori_path_Mac: raw(original) macros dir path example: "./Macro/"
    03(1). path_Raw: raw datas (XXX_Mu.txt) example: "/data4/YuSiang/ExampleSpace/"
    03(2). path_Hk : raw house keeping datas (XXX_Hk.txt) example: "/data4/YuSiang/ExampleSpace/HKData/"

04. The path you want to save the root file of raw data (XXX_Mu.root), it can share with another runs data: 
    path_Rot: example: "/data4/YuSiang/ExampleSpace/rootfile/"
    
04. The path you want to save the operation data and the results: 
    path_OpR: example: "/data4/YuSiang/ExampleSpace/Ana/"

05. The path of the folder which put the analysis "program files" for this analysis:
    path_Mac: default: "./MacroTmp/"

06.Change the Data name want to be researched!! 
    06(0). name_Raw: "*_Mu" find out the raw data: *_Mu.txt
    06(1). name_Hk : "*_HK" find out the house keeping data: *_Mu.txt
    PS: Examples of how to search are put in "MainControl.C"
        A directory contain 3 files:    Name1.txt & Name.txt & N.txt
        Ex1. case for name_Raw = "Name":
            Will analyze the file: Name.txt
        Ex2. case for name_Raw = "Name*":
            Will analyze the file: Name.txt & Name1.txt
        Ex3. case for name_Raw = "*":
            Will analyze the file: Name.txt & Name1.txt % N.txt
        @@@
07.Change the operation mode for you want, and search marker @AdjustOpMode to adjust.
    07(0).testMode        : default: false; //Test code by using small sample number in some pwidth ana. program.
    07(1).normalizeMode   : default: true ; //Normalize the difference by the statistic in pwidth ana. program.
    07(2).NegHiRatChMode  : default: true ; //Could not use now!
    07(3).lowstatisticMode: default: false; //when the data come from large theta, use MuEelID to analyze the pwidth.
    07(4).rootFileMode    : default: true ; //re save the data of pwidth ana. want to use. 
    07(5).SETightMode     : default: true ; //select event tightly for MuEelID in pwidth ana. 
    07(6).PointTextMode   : default: true ; //draw the data point of HK VS Rate to be text(save time).

08.Set if you want to skip some block of program: 
    08(0).bool SkipBlock  : default: true ; 
        true : Skip the programs in the range for: if(SkipBlock==false){} 
        false: Enable all of the programs except the "DSLAna.h"
    08(1).bool ReDSLAna   : default: true ; 
        rerun the DSLAna.h to save DSLData.h when you change anything in "AnaVariable.h" or change the select data.

09.Select the program want to be used: Put the program block to the outer of range for: if(SkipBlock==false){}

10.Go to the path you save the result, and check the result.

11.If the program or the graph has something wrong, or the graph for result has something that needs to improve. Please connect with Yu Siang Xiao.

Thank you for your use.
