
# !!Warning!!: the Maincontrol isn't work without some emvoriment(I guess CVMFS)
### The solution is use **"chip02.phy.ncu.edu.tw"** and add the two line in your **".bashrc"**:
    source /data4/cmkuo/root/bin/thisroot.sh
    source /cvmfs/cms.cern.ch/cmsset_default.sh
# !!! New branch EarlyVer_V3.05V2 (ver20230327) !!!
### 01. Save the Setting file by JSON
#### Modify the underlying logic of the main program parameter passing to store the control parameters of the main program in a JSON file, and read the JSON file when executing the main program.
* As usual, you can edit the path and parameter settings in the **"MainControl()"** function at the bottom to perform analysis.
* After editing the settings in the main program and executing it, a configuration file will be generated. You can save this configuration file or modify its contents for future use.
    + PS: **"OutSettingTmp.JSON"** is the default file for generate in the program.
* There are also some of the setting files that I used for analysis in the main dictionary like AdjustPath-*.JSON.
* If you already have a JSON setting , then execute the program by main program **"MainControl(\*Settingfile)"**.
* @graph 1
### 02. Add the Setting of Monticello: MCMode, path_MC, name_MC
* This new Setting means the code operation by **MCMode** or not.
* **"path_MC"** is the path of MC source file.
* **"name_MC"** is the name of MC source file.
* **"path_Rot"** will be changed from where Raw_Mu.root is stored to where RAW_MC.root is stored under the MCMode.
* If **"MCMMode"** is enabled, the program will not read the contents of path_Raw, path_Hk, name_Raw, and name_Hk, which must be set to empty values. Conversely, if MCMMode is disabled, the program will not read the contents of path_MC and name_MC.
### 03. Split the header file AnaVariable.h into two parts.
* **"AnaVariable.h"** mainly stores the analysis parameters.
* **"./DetInforSet/DetSet\*.h"** is the detector model and mechanical structure parameter, and is related to the **"det_ver"**.
* **"./DetInforSet/DetSet/DetSetUserSet.h"** is a file that provides users with the ability to customize the detector structure or module count during testing.
    + PS: When using DetSetUserSet.h, the det_ver should be **"User"**.
* After the operate RenewMacros.h, the AnaVariable.h and ./DetInforSet/DetSet\*.h will be will be merged into **./Macro/AnaVariable.h**.
### 04. Fix some issues with fetching RunData files
* **Warning**: all Run files in the **"path_Run/"** directory cannot have only Run0, otherwise it will cause errors.
### 05. **DataNameAnaV2.C** 
+ add function MCNameAnaRoot_Mu to fetch MC files **"path_MC/name_MC"**.
### 06. **DSLAnaV4.C** 
+ add the attributes for the MCMode
### 07. **G4CaConvertorV2.C**
+ Use the script to generate files **"RAW_MC.root"** from **"path_MC/name_MC"**.
### 08. **"EventAnaV2.C"** & **DayEAnaV4.C**
+ added logic for handling MC files. 
### 09. **"SettingJSONIO.h"** 
+ Added IO header file for JSON parameter settings.
### 10. **"EffTestV2_V3.txt"**
+ Full effciency fake data.
+ File for scintillator saturation efficiency (used for testing scintillator efficiency to avoid errors). The BID from 1-128
### 11. **"./Macro/GA4x4_2.csv"**
+ Using the detector GA from V3.0 to fill in the missing parts of the detector GA for V2.0.
### 12. **"GIDConvert.xlsx"**
+ If G4GID and the experimental setup GID do not match, to avoid the need to generate an additional set of MC file-based scintillator position configuration files, you can directly convert them through this spreadsheet. Let me know if you need help.
# ver20230323:
  **Revise the issue regarding the inconvenient use of the 'BDINV[]' array in 'AnaVariable.h' by replacing its function with 'int BDCheck(int b)' from 'GobelFunctions.h'.**
  * You can download the /Macros and cover the original /Macros on your computer. **Then, you could neglect the array BDINV[].**
# Introduction of MuographyAnaV3.05Det-
Public Muography Analysis Code for Detector V2.0, V3.00, V3.05.

This is the analysis program of Muography for Detector V2.0~ V3.05 made by Yu Siang Xiao.

Q: How to use?  

### 00. Download the "MainControl.C" , "AnaVariable.h" , RenewMacros.h & "/Macro" onto the NAS.
    Ps: If you wan use multiple cpu to analyze the different "run" data parallelly. 
    You can download the Run.C to same dir, and set the parameter like the MainControl.C.

### 01. Revise the analysis variable in the "AnaVariable.h". 
    change the trigger for anylize, color palette, default fitting function parameter
      example: detector & scintillator size and numbers, runs time range.

### 02. Now, You can change the information in the "MainControl.C". Find the marker @AdjustPath by searching in the file.

### 03. Changing the path of floder which "input files" and want to do analysis for: 
    03(0). Ori_path_Mac: raw(original) macros dir path example: "./Macro/"
    03(1). path_Raw: raw datas (XXX_Mu.txt) example: "/data4/YuSiang/ExampleSpace/"
    03(2). path_Hk : raw house keeping datas (XXX_Hk.txt) example: "/data4/YuSiang/ExampleSpace/HKData/" 

### 04. The path you want to save the root file of raw data (XXX_Mu.root), it can share with another runs data: 
    04(0). path_Rot: example: "/data4/YuSiang/ExampleSpace/rootfile/"
    04(1). The path you want to save the operation data and the results: 
    04(2). path_OpR: example: "/data4/YuSiang/ExampleSpace/Ana/"

### 05. The path of the folder which put the analysis "program files" for this analysis:
    path_Mac: default: "./MacroTmp/"

### 06.Change the Data name want to be researched!! 
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

### 07.Change the operation mode for you want, and search marker @AdjustOpMode to adjust.
    07(0).testMode        : default: false; //Test code by using small sample number in some pwidth ana. program.
    07(1).normalizeMode   : default: true ; //Normalize the difference by the statistic in pwidth ana. program.
    07(2).NegHiRatChMode  : default: true ; //Could not use now!
    07(3).lowstatisticMode: default: false; //when the data come from large theta, use MuEelID to analyze the pwidth.
    07(4).rootFileMode    : default: true ; //re save the data of pwidth ana. want to use. 
    07(5).SETightMode     : default: true ; //select event tightly for MuEelID in pwidth ana. 
    07(6).PointTextMode   : default: true ; //draw the data point of HK VS Rate to be text(save time).

### 08.Set if you want to skip some block of program: 
    08(0).bool SkipBlock  : default: true ; 
        true : Skip the programs in the range for: if(SkipBlock==false){} 
        false: Enable all of the programs except the "DSLAna.h"
    08(1).bool ReDSLAna   : default: true ; 
        rerun the DSLAna.h to save DSLData.h when you change anything in "AnaVariable.h" or change the select data.

### 09.Select the program want to be used: Put the program block to the outer of range for: if(SkipBlock==false){}

### 10.Go to the path you save the result, and check the result.

### 11.If the program or the graph has something wrong, or the graph for result has something that needs to improve. Please connect with Yu Siang Xiao.

Thank you for your use.
