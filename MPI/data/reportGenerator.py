import os
import csv
from docx import Document
from docx.shared import Inches

files=['naiveOutput.txt']
data=[]
for file in files:
	with open(file,'r') as csvfile:
		cursor = csv.reader(csvfile, delimiter=' ')
		for row in cursor:
			data.append(row)
cleanedData=[[] for _ in range(len(files)+1) ]
print(cleanedData)
i=0
while i<len(data):
	cleanedData[0].append(float(data[i][2]))
	cleanedData[1].append(float(data[i+1][4]))
	i=i+2
print(data)
print(cleanedData)
try:
	os.remove('report.docx')
except OSError:
	pass

document = Document()

document.add_heading('Parallel Computing Assignment', 0)

p = document.add_paragraph('')
p.add_run('Roll No. - ').bold = True
p.add_run(' 160123005 ')

# records = (
#     (3, '101', 'Spam'),
#     (7, '422', 'Eggs'),
#     (4, '631', 'Spam, spam, eggs, and spam')
# )

table = document.add_table(rows=1, cols=len(files)+1)

hdr_cells = table.rows[0].cells
hdr_cells[0].text = 'Size(n)'
for i in range(0,len(files)):
	hdr_cells[i+1].text=files[i].split(".")[0]

for qty, id, desc in records:
    row_cells = table.add_row().cells
    row_cells[0].text = str(qty)
    row_cells[1].text = id
    # row_cells[2].text = desc

document.add_page_break()

document.save('report.docx')			

# p.add_run('italic.').italic = True

# document.add_heading('Heading, level 1', level=1)

# document.add_paragraph(
#     'first item in unordered list', style='List Bullet'
# )
# document.add_paragraph(
#     'first item in ordered list', style='List Number'
# )

# document.add_picture('monty-truth.png', width=Inches(1.25))
