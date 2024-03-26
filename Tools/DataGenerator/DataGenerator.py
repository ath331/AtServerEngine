from openpyxl import load_workbook


#import os
#import glob
#from openpyxl import load_workbook

#def process_xlsx_files(root_path):
#    for root, dirs, files in os.walk(root_path):
#        for file in files:
#            if file.endswith(".xlsx"):
#                file_path = os.path.join(root, file)
#                # ���� ���� ó���� ���� �۾� ����
#                # ���⼭�� openpyxl�� ����Ͽ� ������ ������ ���Դϴ�.
#                wb = load_workbook(file_path)
#                # ���ϴ� �۾��� ����
#                # ��: ��Ʈ �б�, ������ ó�� ��

## Ư�� ��ο��� �����Ͽ� ��� ���� ��ο��� xlsx ���Ͽ� ����
#root_path = "/your/root/path"
#process_xlsx_files(root_path)

def read_excel_sheet(filename, sheetname):

    filename = filename + '.xlsx'

    workbook = load_workbook(filename)

    try:
        sheet = workbook[sheetname]

        for row in sheet.iter_rows(values_only=True):
            for cell in row:
                print(cell, end=" ")
            print()
            
    except KeyError:
        print(f"Invalid Sheet!! Error Sheet Name : '{sheetname}'")

    finally:
        # ���� �ݱ�
        workbook.close()


# ���� �̸��� ��Ʈ �̸� �Է�
filename = input("File Name? : ")
sheetname = input("Sheet name? : ")


# �Լ� ȣ��
read_excel_sheet(filename, sheetname)