sleep 0 &&root -l -b MainControl.C\(\"./JSONEg/gdml_ele_E5.0E+09P0110M11110.json\"\) &
sleep 0 &&root -l -b MainControl.C\(\"./JSONEg/gdml_mu_E5.0E+09P0110M11110.json\"\) &
wait
echo "Finish the multiple execute of programs."
