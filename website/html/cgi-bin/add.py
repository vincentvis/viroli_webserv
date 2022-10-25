import cgi, cgitb
import os
import sys

cgitb.enable()
input_data = cgi.FieldStorage()

print("Content-Type: text/html") # HTML is following
print("")                         # Leave a blank line


print("""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
""")

print("<h1>val1: {0}</h1>".format(input_data.getvalue("val1")))
print("<h1>val2: {0}</h1>".format(input_data.getvalue("val2")))

print ("Number of arguments:", len(sys.argv), "arguments")
print ("Argument List:", str(sys.argv))

try:
    num1 = int(input_data.getvalue("val1"))
    num2 = int(input_data.getvalue("val2"))
except:
    print("<h1>Sorry, the script cannot turn your inputs into numbers (integers).</h1>\n</body>\n</html>")
    raise SystemExit(0)

print("<h1>{0} + {1} = {2}</h1>".format(num1, num2, num1 + num2))

print("""</body>
</html>""")