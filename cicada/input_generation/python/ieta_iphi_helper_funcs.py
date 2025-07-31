import math

def get_iphi(phi): 
        if phi >= 0:
                iphi = int(math.floor(phi * 72. / (2. * math.pi)))+1 #[1,36]
        elif phi < 0:
                iphi = 72 + int(math.floor(phi * 72. / (2. * math.pi)))+1 #[37,72]
        else: return -999
        iphi = (iphi+1)%72 # shifting the ends-> brings 71->0,72->1,1->2,2->3,70->71 
        iphi = int(iphi/4) # [71,72,1,2]->0,[3,4,5,6]->1,...,[67,68,69,70]->17
        return iphi

def get_ieta(eta):
        eta_boundaries = [
                0.087,
                0.174,
                0.261,
                0.348,
                0.435,
                0.522,
                0.609,
                0.696,
                0.783,
                0.87,
                0.957,
                1.044,
                1.131,
                1.218,
                1.305,
                1.392,
                1.479,
                1.566,
                1.653,
                1.74,
                1.83,
                1.93,
                2.043,
                2.172,
                2.322,
                2.5,
                2.65,
                2.868
        ]
        if eta >= 0: factor = 1
        elif eta < 0: factor = -1
        for n in range(28):
                if abs(eta) < eta_boundaries[n]:
                        ieta = (n+1) # ieta in [1,28]
                        ieta = factor*(int((ieta-1)/4)+1) #ieta in [-7,-1],[1,7] 
                        if(ieta>=0): ieta += 6
                        elif(ieta<0): ieta +=7
                        return ieta #ieta in [0,6],[7,13] 
        return -999