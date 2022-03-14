#!/usr/bin/env python3
# use this script to get a tag cloud picture of all PDF files stored in a given folder
# usage: ./script folder
# you may need these 2 modules: pyPDF2 and worcloud
# pip intall pyPDF2
# pip install worcloud

# importing all required modules
import PyPDF2
from PyPDF2 import PdfFileReader
from wordcloud import WordCloud, STOPWORDS
import matplotlib.pyplot as plt
# import pandas as pd
from pathlib import Path
import os, fnmatch
import sys

folder=sys.argv[1]
#folder = '/home/rpires/Downloads/nn/'
#folder = '/home/rpires/NOKIA/CSS/bulletins/'

def tagcloud(pdffile):
    # openig each pdf file
    file = open(folder + pdffile, 'rb')
    inputPdf = PyPDF2.PdfFileReader(file)
    # converting the contents of all pages to a string
    text = ''
    for i in range(0, inputPdf.numPages):
        pageObj = inputPdf.getPage(i)
        text = text + pageObj.extractText()
    #print(text)
    # ideas https://www.geeksforgeeks.org/generating-word-cloud-python/
    # https://stackoverflow.com/questions/25346058/removing-list-of-words-from-a-string
    comment_words = ''
    newstopwords = ['bulletin', 'internal', 'subscribe', 'unsubscribe', 'nokia']
    stopwords = set(STOPWORDS)
    #stopwords.add('bulletin')
    #stopwords.add('subscribe')
    # I want to add a list of stop words but it fails!
    for i in range(len(newstopwords)):
        stopwords.add(newstopwords[i])
    # iterate through the string, which I need to split in words first
    for val in text.split():
        # split the value
        tokens = val.split()
        # Converts each token into lowercase
        for i in range(len(tokens)):
            tokens[i] = tokens[i].lower()

        comment_words += " ".join(tokens) + " "
    wordcloud = WordCloud(width=800, height=800,
                          background_color='white',
                          stopwords=stopwords,
                          min_font_size=10).generate(comment_words)
    # plot the WordCloud image
    plt.figure(figsize=(8, 8), facecolor=None)
    plt.imshow(wordcloud)
    plt.axis("off")
    plt.tight_layout(pad=0)
    imagefile = Path(pdffile).stem + ".png"
    plt.savefig(folder + imagefile, dpi=None, facecolor='w', edgecolor='w',
                orientation='portrait', papertype=None, format=None,
                transparent=False, bbox_inches=None, pad_inches=0.1,
                frameon=None)
    # plt.show()

listOfFiles = os.listdir(folder)
pattern = "*.pdf"
for pdffile in listOfFiles:
    if fnmatch.fnmatch(pdffile, pattern):
        print('Creating a tag cloud for the file', pdffile)
        tagcloud(pdffile)
