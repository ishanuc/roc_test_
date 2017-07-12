#!/usr/bin/python
import latexfig as lfg
from sklearn import metrics
import pandas as pd
import seaborn as sns

import matplotlib.pyplot as plt
import sys
import scipy.stats as stat 
import subprocess
from sklearn.metrics import roc_curve, auc


def roc_(d,TH=2,ax='',week=''):
    fpr, tpr, _ = roc_curve(d.flu.values,d.dec.values)
    roc_auc = auc(fpr, tpr)
    if ax=='':
        plt.figure(figsize=(4,4))
    else:
        plt.sca(ax)
    lw = 2
    plt.plot(fpr, tpr, color='darkorange',
         lw=lw, label='ROC curve (area = %0.2f)' % roc_auc)
    plt.plot([0, 1], [0, 1], color='navy', lw=lw, linestyle='--')
    plt.xlim([0.0, 1.0])
    plt.ylim([0.0, 1.05])
    plt.xlabel('False Positive Rate',fontweight='bold',fontsize=12)
    plt.ylabel('True Positive Rate',fontweight='bold',fontsize=12)
    plt.title('week:'+str(week),y=1.04,fontweight='bold',fontsize=12)
    plt.legend(loc="lower right")
    return fpr,tpr,auc


def read_roc(filename,week,TH=2,ax='',PLT=False):
    d=pd.read_csv(filename,header=None,sep=" ")
    d.columns=['flu','dec']
    
    for i in d[d['flu'] < TH].index:
        d.iloc[i]=[0,d.loc[i].dec]
    for i in d[d['flu'] >= TH].index:
        d.iloc[i]=[1,d.loc[i].dec]
        
    if PLT:
        plt.figure(figsize=(15,5))
        plt.subplot(1, 2, 1)
        plt.plot(d.flu.values,label='flu')
        plt.subplot(1,2,2)
        plt.plot(d.dec.values,label='dec')
        plt.legend()
        
    tpr,fpr,auc=roc_(d,TH,ax,week)
    return tpr,fpr,auc



fd=pd.read_csv('./code/filenames.txt',header=None)
fd.columns=['files']
fnames=fd.values

font = {#'family' : 'normal',
        'weight' : 'bold',
        'size'   : 32}

lfg.mpl.rc('font', **font)

fd=pd.read_csv('./code/filenames2.txt',header=None)
fd.columns=['files']
fnames=fd.values

sns.set_style('darkgrid')
#fig=plt.figure(figsize=(10,10))
fig,ax=lfg.newfig(3)
fig.subplots_adjust(hspace=0.2)
fig.subplots_adjust(wspace=.1)
FS=10
FS1=10

R=3
C=3

for num in lfg.np.arange(R*C):
    i=num/C
    j=num-(i*C)    
    
    ax = plt.subplot2grid((3,3),(i,j), colspan=1,rowspan=1)
    read_roc('./code/'+fnames[num][0],int(fnames[num][0].replace('res_','')),TH=5,ax=ax,PLT=False)
    ax.tick_params(labelsize=10)

    if i<2:
        ax.set_xticklabels([])
        ax.set_xlabel('')
    if j>0:
        ax.set_yticklabels([])
        ax.set_ylabel('')
            
#plt.savefig('PRED_5'+'.pdf',dpi=1200, bbox_inches='tight')
lfg.savefig('PRED_5')
