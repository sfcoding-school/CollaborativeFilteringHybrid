def scriviLog(filename, quelloCheVuoiScrivere):
    f = open("dataset/log/report" + filename.replace("dataset/test/", ""), 'ab')
    f.write(quelloCheVuoiScrivere + "\n")
