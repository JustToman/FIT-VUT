from scipy.io import wavfile
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import lfilter
import cmath

#Vlastní implementace DFT
def DFT(frame):
    dft = list()
    frame_in = list()
    frame_in.extend(frame) #Překopírování rámce
    frame_in.extend([0]*(1024-len(frame))) #Rozšíření rámce
    for k in range(len(frame_in)):
        sum_of = complex()
        for n in range(len(frame_in)):
            sum_of += frame_in[n]*cmath.exp(-2j*cmath.pi*n*k/1024)
        dft.append(sum_of)
    return dft


# Vlastní implementace IDTF
def IDFT(frame):
    idft = list()
    frame_in = list()
    frame_in.extend(frame)
    frame_in.extend([0]*(1024-len(frame)))
    for n in range(len(frame)):
        sum_of = 0
        for k in range(len(frame)):
            sum_of += frame[k]*cmath.exp(2j*cmath.pi*n*k/1024)
        idft.append(sum_of/1024)
    return idft
            

#Načtení dat
fs_moff_tone, data_moff_tone = wavfile.read("../audio/maskoff_tone.wav") 
fs_mon_tone, data_mon_tone = wavfile.read("../audio/maskon_tone.wav")
fs_moff_sen, data_moff_sen = wavfile.read("../audio/maskoff_sentence.wav")
fs_mon_sen, data_mon_sen = wavfile.read("../audio/maskon_sentence.wav")

#Ořezání tónu na 1s
data_mon_tone = data_mon_tone[:16000]
data_moff_tone = data_moff_tone[:16000]

#Vypočtení průměrů a maxim nahrávek
data_moff_tone_mean = np.mean(data_moff_tone)
data_moff_tone_max = np.abs(data_moff_tone).max()
data_moff_tone_frames = list()

data_mon_tone_mean = np.mean(data_mon_tone)
data_mon_tone_max = np.abs(data_mon_tone).max()
data_mon_tone_frames = list()

data_moff_sen_mean = np.mean(data_moff_sen)
data_moff_sen_max = np.abs(data_moff_sen).max()
data_moff_sen_frames = list()

data_mon_sen_mean = np.mean(data_mon_sen)
data_mon_sen_max = np.abs(data_mon_sen).max()
data_mon_sen_frames = list()

data_moff_tone = data_moff_tone.tolist()
data_mon_tone = data_mon_tone.tolist()
data_moff_sen = data_moff_sen.tolist()
data_mon_sen = data_mon_sen.tolist()

#Ustredneni a normalizovani

for i in range(len(data_moff_tone)):
    data_moff_tone[i] -= data_moff_tone_mean
    data_moff_tone[i] /= data_moff_tone_max
    data_mon_tone[i] -= data_mon_tone_mean
    data_mon_tone[i] /= data_mon_tone_max

for i in range(len(data_moff_sen)):
    data_moff_sen[i] -= data_moff_sen_mean
    data_moff_sen[i] /= data_moff_sen_max

for i in range(len(data_mon_sen)):
    data_mon_sen[i] -= data_mon_sen_mean
    data_mon_sen[i] /= data_mon_sen_max

#Rozdeleni na ramce
for i in range(0,len(data_moff_tone),160):
    data_moff_tone_frames.append(data_moff_tone[i:i+320])

for i in range(0,len(data_mon_tone),160):
    data_mon_tone_frames.append(data_mon_tone[i:i+320])

#Zkrácení na 99 rámců
data_moff_tone_frames = data_moff_tone_frames[:99]
data_mon_tone_frames = data_mon_tone_frames[:99]

#Vyberu ramec a provedu na nem centralni klipovani

# 70% clipping mask off tone
clipped_moff_frames = list()
for j in range(len(data_moff_tone_frames)):
    clipped_moff_frames.append(list())
    data_moff_tone_max = np.abs(data_moff_tone_frames[j]).max()
    for i in data_moff_tone_frames[j]:
        if i > 0.7*data_moff_tone_max:
            clipped_moff_frames[j].append(1.0)
        elif i < -0.7*data_moff_tone_max:
            clipped_moff_frames[j].append(-1.0)
        else:
            clipped_moff_frames[j].append(0.0)
    clipped_moff_frames[j] = np.array(clipped_moff_frames[j])

#70% clipping mask on
clipped_mon_frames = list()
for j in range(len(data_mon_tone_frames)):
    clipped_mon_frames.append(list())
    data_mon_tone_max = np.abs(data_mon_tone_frames[j]).max()
    for i in data_mon_tone_frames[j]:
        if i > 0.7*data_mon_tone_max:
            clipped_mon_frames[j].append(1.0)
        elif i < -0.7*data_mon_tone_max:
            clipped_mon_frames[j].append(-1.0)
        else:
            clipped_mon_frames[j].append(0.0)
    clipped_mon_frames[j] = np.array(clipped_mon_frames[j])

#AUTOKORELACE
correlated_moff_tone_frames = list()
correlated_mon_tone_frames = list()

lag_mon_tone_frames = list()
lag_moff_tone_frames = list()

f0_mon_tone_frames = list()
f0_moff_tone_frames =list()


#MASKOFF TONE
for i in range(len(clipped_moff_frames)):
    correlated_moff_tone_frames.append(list())
    for j in range(len(clipped_moff_frames[i])):
        rv = 0
        for n in range(len(clipped_moff_frames[i])):
            k_index = n + j
            if k_index >= 320:
                rv += 0
            else:
                rv += clipped_moff_frames[i][n] * clipped_moff_frames[i][k_index]
        correlated_moff_tone_frames[i].append(rv) 
    lag_moff_tone_frames.append(np.argmax(correlated_moff_tone_frames[i][32:])+32)
    f0_moff_tone_frames.append(fs_moff_tone/lag_moff_tone_frames[i])

#MASKON TONE
for i in range(len(clipped_mon_frames)):
    correlated_mon_tone_frames.append(list())
    for j in range(len(clipped_mon_frames[i])):
        rv = 0
        for n in range(len(clipped_mon_frames[i])):
            k_index = n + j
            if k_index >= 320:
                rv += 0
            else:
                rv += clipped_mon_frames[i][n] * clipped_mon_frames[i][k_index]
        correlated_mon_tone_frames[i].append(rv) 
    lag_mon_tone_frames.append(np.argmax(correlated_mon_tone_frames[i][32:])+32)
    f0_mon_tone_frames.append(fs_mon_tone/lag_mon_tone_frames[i])

f0_mon_tone_mean = np.mean(f0_mon_tone_frames)
f0_mon_tone_var  = np.var(f0_mon_tone_frames)
f0_moff_tone_mean = np.mean(f0_moff_tone_frames)
f0_moff_tone_var = np.var(f0_moff_tone_frames)
            
#DFT MASKOFF
moff_tone_frames_DFT = list() 
moff_tone_frames_DFT_test = list() 


for i in range(len(data_moff_tone_frames)): 
   moff_tone_frames_DFT.append(DFT(data_moff_tone_frames[i]))
   #moff_tone_frames_DFT.append(np.fft.fft(data_moff_tone_frames[i],n=1024))

#Úprava koeficientů do grafu
moff_tone_frames_DFT_coef = list()
for i in range(len(moff_tone_frames_DFT)):
    moff_tone_frames_DFT_coef.append(list())
    for j in range(512):
        coef = 10 * (np.log10((np.abs(moff_tone_frames_DFT[i][j])**2)))
        moff_tone_frames_DFT_coef[i].append(coef)


#DFT MASKON
mon_tone_frames_DFT = list() 
for i in range(len(data_mon_tone_frames)):
    mon_tone_frames_DFT.append(DFT(data_mon_tone_frames[i]))
    #mon_tone_frames_DFT.append(np.fft.fft(data_mon_tone_frames[i],n=1024))

#Úprava koeficientů do grafu
mon_tone_frames_DFT_coef = list()
for i in range(len(mon_tone_frames_DFT)):
    mon_tone_frames_DFT_coef.append(list())
    for j in range(512):
        coef = 10 * (np.log10((np.abs(mon_tone_frames_DFT[i][j])**2)))
        mon_tone_frames_DFT_coef[i].append(coef)
    
moff_tone_frames_DFT_coef = np.array(moff_tone_frames_DFT_coef)
mon_tone_frames_DFT_coef = np.array(mon_tone_frames_DFT_coef)

#Úkol 6
freq_char = list()
freq_char = np.array(freq_char)
freq_char  = np.divide(mon_tone_frames_DFT,moff_tone_frames_DFT)
freq_char = np.mean(np.abs(freq_char),axis=0)
freq_char_bef = list()
freq_char_bef.extend(freq_char)
for i in range(512):
    freq_char[i] = 10*(np.log10(np.abs(freq_char[i])**2))

#Úkol 7
imp_odezva = np.fft.ifft(freq_char_bef[0:512],1024)
imp_odezva2 = IDFT(freq_char_bef[0:512])  
imp_odezva2 = np.array(imp_odezva2)   
 

#Úkol 8
filtered_mask_off = lfilter(imp_odezva2[0:512],[1.0],data_moff_sen)
filtered_mask_off_tone = lfilter(imp_odezva2[0:512],[1.0],data_moff_tone)
wavfile.write("../audio/sim_maskon_sentence.wav",16000,filtered_mask_off.real)
wavfile.write("../audio/sim_maskon_tone.wav",16000,filtered_mask_off_tone.real)

t = np.arange(clipped_moff_frames[0].size)/ fs_mon_tone
    
fig, ax = plt.subplots(4,1,figsize=(12,6))
fig2,ax2 = plt.subplots(2,1)
k = np.arange(0, 320)
Rv = correlated_mon_tone_frames[0]
a = np.arange(0,99)

ax[0].set_title('Rámec')
ax[0].set_xlabel('time')
ax[0].plot(t,data_moff_tone_frames[0])
ax[0].set_ylabel("y")
ax[1].set_title('Zvukový signál')
ax[1].set_xlabel('t[s]')
ax[1].plot(t,clipped_moff_frames[0])
ax[1].set_ylabel("y")
ax[2].set_title('Autokorelace')
ax[2].set_xlabel('vzorky')
ax[2].plot(k,Rv)
ax[2].axvline(32,color = "black",label = "Práh")
ax[2].stem([lag_moff_tone_frames[0]+4],[correlated_moff_tone_frames[0][lag_moff_tone_frames[0]]] ,linefmt='r-',label = "lag")
ax[2].set_ylabel("y")
ax[2].legend(loc = 0)
ax[3].set_title('Základní frekvence rámců')
ax[3].set_xlabel('rámce')
ax[3].set_ylabel("f0")
ax[3].plot(a,f0_moff_tone_frames,label="bez roušky")
ax[3].plot(a,f0_mon_tone_frames, label="s rouškou")
ax[3].legend(loc = 'upper right')

ax2[0].set_title("Spektrogram bez roušky")
ax2[0].set_xlabel("Time")
ax2[0].set_ylabel("Frekvence")
im_ax0 = ax2[0].imshow(moff_tone_frames_DFT_coef.T[::-1],aspect='auto',extent=[0,1,0,8000],vmin = moff_tone_frames_DFT_coef.min(),vmax = moff_tone_frames_DFT_coef.max())
ax2[1].set_title("Spektrogram s rouškou")
ax2[1].set_xlabel("Time")
ax2[1].set_ylabel("Frekvence")
im_ax1 = ax2[1].imshow(mon_tone_frames_DFT_coef.T[::-1],aspect='auto',extent=[0,1,0,8000],vmin = mon_tone_frames_DFT_coef.min(),vmax = mon_tone_frames_DFT_coef.max())
fig2.colorbar(im_ax0,ax=ax2[0])
fig2.colorbar(im_ax1,ax=ax2[1])
fig2.tight_layout()


fig3, ax3 = plt.subplots()
freq_x = np.arange(0,512)
ax3.plot(freq_x,freq_char[:512].real)
ax3.set_title("Frekvenční charakteristika roušky")
ax3.set_xlabel("Vzorky")
ax3.set_ylabel("y")
fig3.tight_layout()

fig4, ax4 = plt.subplots(4,1,figsize=(10,10))
ax4[3].plot(data_mon_sen,label = "věta - s rouškou")
ax4[3].plot(data_moff_sen,label = "věta - bez roušky")
ax4[3].plot(filtered_mask_off, label = 'věta - simulace roušky')
ax4[3].legend()

ax4[0].plot(data_mon_sen)
ax4[1].plot(data_moff_sen)
ax4[2].plot(filtered_mask_off)
ax4[0].set_title("Věta s rouškou")
ax4[1].set_title("Věta bez roušky")
ax4[2].set_title("Věta - simulace roušky")
ax4[3].set_title("Porovnání signálů po použití filtru")
ax4[0].set_xlabel("Vzorky")
ax4[1].set_xlabel("Vzorky")
ax4[2].set_xlabel("Vzorky")
ax4[3].set_xlabel("Vzorky")
ax4[0].set_ylabel("y")
ax4[1].set_ylabel("y")
ax4[2].set_ylabel("y")
ax4[3].set_ylabel("y")

fig5, ax5 = plt.subplots(2,1)
ax5[0].plot(t,data_moff_tone_frames[0])
ax5[0].set_title("Rámec tónu bez roušky")
ax5[0].set_xlabel("t[s]")
ax5[0].set_ylabel("y")
ax5[1].plot(t,data_mon_tone_frames[0])
ax5[1].set_title("Rámec tónu s rouškou")
ax5[1].set_xlabel("t[s]")
ax5[1].set_ylabel("y")

fig6, ax6 = plt.subplots()
ax6.plot(imp_odezva2.real[0:512])
#ax6.plot(imp_odezva.real[0:512])
ax6.set_title("Impulsní odezva")
ax6.set_xlabel("Vzorky")
ax6.set_ylabel("y")

fig7, ax7 = plt.subplots(4,1,figsize=(10,10))
ax7[3].plot(data_mon_tone,label = "tón - s rouškou")
ax7[3].plot(data_moff_tone,label = "tón - bez roušky")
ax7[3].plot(filtered_mask_off_tone, label = 'tón - simulace roušky')
ax7[3].legend()

ax7[0].plot(data_mon_tone)
ax7[1].plot(data_moff_tone)
ax7[2].plot(filtered_mask_off_tone)
ax7[0].set_title("Tón s rouškou")
ax7[1].set_title("Tón bez roušky")
ax7[2].set_title("Tón - simulace roušky")
ax7[3].set_title("Porovnání signálů po použití filtru")
ax7[0].set_xlabel("Vzorky")
ax7[1].set_xlabel("Vzorky")
ax7[2].set_xlabel("Vzorky")
ax7[3].set_xlabel("Vzorky")
ax7[0].set_ylabel("y")
ax7[1].set_ylabel("y")
ax7[2].set_ylabel("y")
ax7[3].set_ylabel("y")


fig7.tight_layout()
fig6.tight_layout()
fig5.tight_layout()
fig4.tight_layout()
fig.tight_layout()
#fig4.savefig("vety.png")
#fig2.savefig("spect.png")
#fig5.savefig("frames.png")
#fig3.savefig("frekv_char.png")
#fig.savefig("tony.png")
#fig6.savefig("imp_odezva.png")
#ig7.savefig("por_tonu.png")

plt.show()