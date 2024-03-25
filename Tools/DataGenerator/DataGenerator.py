from openpyxl import load_workbook


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