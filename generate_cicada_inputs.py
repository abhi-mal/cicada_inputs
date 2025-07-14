import os 

input_base_path = '/code/input_files/'
processes_to_run = ['QCD','ttbar']
is_data= 'False'
base_cmd = 'cmsRun cicada/input_generation/python/makeCICADANtuplesFromRAW.py isData=%s inputFiles=file:'%is_data
output_folder = '/code/output_files/'
for process_to_run in processes_to_run:
        input_path = input_base_path + process_to_run + '/'
        for filename in os.listdir(input_path):
                if '.root' in filename:
                        file_path = input_path + filename
                        output_filename= 'processed_%s_%s'%(process_to_run,filename.replace('.root',''))
                        cmd = base_cmd + file_path + ' outputFile=%s'%output_filename
                        print("Getting inputs from %s:%s"%(process_to_run,filename))
                        os.system(cmd)
                        os.system('mv %s.root %s/%s/'%(output_filename,output_folder,process_to_run))

