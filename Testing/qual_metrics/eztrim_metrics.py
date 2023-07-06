count = 0

adaptors = 0
reads = 0

q20 = 0
q30 = 0
bases = 0

record = []
quality = []
with open ("ezTrim.fastq", 'r') as file:
    for line in file:
        if count == 4:
            if "AGATCGGAAGAGCACACGTCTGAACTCCAGTCA" in record[1]:
                adaptors = adaptors + 1
            for index in range(len(record[1])):
                if record[1][index] != 'N':
                    bases = bases + 1
                    if int(ord(record[3][index]) - 33) > 19:
                        q20 = q20 + 1
                    if int(ord(record[3][index]) - 33) > 29:
                        q30 = q30 + 1
            count = 0
            record = []
            reads = reads + 1
        record.append(line)
        count = count + 1
        
q20_score = q20/bases
q30_score = q30/bases
adaptor_content = adaptors/reads

print("-----------------------------------------------------------------------------------------")
print("EZTRIM")
print("Q20 SCORE: " + str(q20_score))
print("Q30 SCORE: " + str(q30_score))
print("Adapter Content: " + str(adaptor_content))
print(adaptors)
print("-----------------------------------------------------------------------------------------")