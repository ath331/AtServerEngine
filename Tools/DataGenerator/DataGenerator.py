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
        # 파일 닫기
        workbook.close()


# 파일 이름과 시트 이름 입력
filename = input("File Name? : ")
sheetname = input("Sheet name? : ")


# 함수 호출
read_excel_sheet(filename, sheetname)