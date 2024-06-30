from openpyxl import load_workbook


import argparse
import os
import glob
import jinja2


def main():

    arg_parser = argparse.ArgumentParser(description = 'DataGenerator')
    arg_parser.add_argument('--path', type=str, default='../../Data', help='data dir path')
    args = arg_parser.parse_args()

    for root, dirs, files in os.walk(args.path):
        for file in files:
            if file.endswith(".xlsx"):
                file_path = os.path.join(root, file)

                # xlsx
                workbook = load_workbook(file_path, read_only=True)

                for sheetname in workbook.sheetnames:

                    # sheet
                    sheet = workbook[sheetname]

                    # row
                    for row in sheet.iter_rows(values_only=True):
                        if all(value is None for value in row):
                            break
                        for cell in row:
                            print(cell, end=" ")
                        print()
                        
                workbook.close()


    file_loader = jinja2.FileSystemLoader('Templates')
	env = jinja2.Environment(loader=file_loader)

	template = env.get_template('InfoManagerTemplate.h')
	output = template.render(parser=parser, output=args.output)

	f = open(args.output+'.h', 'w+')
	f.write(output)
	f.close()

	print(output)
                
    return



if __name__ == '__main__':
	main()