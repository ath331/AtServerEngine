from openpyxl import load_workbook


#import os
#import glob
#from openpyxl import load_workbook

#def process_xlsx_files(root_path):
#    for root, dirs, files in os.walk(root_path):
#        for file in files:
#            if file.endswith(".xlsx"):
#                file_path = os.path.join(root, file)
#                # 엑셀 파일 처리를 위한 작업 수행
#                # 여기서는 openpyxl을 사용하여 간단히 예제를 보입니다.
#                wb = load_workbook(file_path)
#                # 원하는 작업을 수행
#                # 예: 시트 읽기, 데이터 처리 등

## 특정 경로에서 시작하여 모든 하위 경로에서 xlsx 파일에 접근
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
        # 파일 닫기
        workbook.close()


# 파일 이름과 시트 이름 입력
filename = input("File Name? : ")
sheetname = input("Sheet name? : ")


# 함수 호출
read_excel_sheet(filename, sheetname)