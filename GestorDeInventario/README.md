# THE BASIC INVENTORY MANAGEMENT
#### Video Demo:  <https://youtu.be/058fS_7JOI0>
#### Description:
The Basic Inventory Management project provides small business owners with a viable way to manage their inventory. To achieve this, a series of interconnected pages were implemented, allowing users to add products, purchase products, search for specific products, and record sales. This is all supported by a database called "Inventory." This database contains four tables, each regulating or controlling a specific aspect. The first table, "Suppliers," is used to record who sells the products. Another very important table, and the one on which the logic of this software is primarily focused, is the products table. Here, we record the product name, the description (specifying the size of the item or other characteristics), its category (which allows us to group it and determine which shelf it belongs on), the unit price (i.e., how much we will sell the product for in the store), and its stock level: the number of items or units we currently have of that specific product.
The next table we will find in our database is the purchases table. In this table, we record the product, the supplier (who sold us the product), the cost per unit, the quantity we are purchasing (i.e., how many units of that product we are buying), and, of course, the date, to keep track of when, on what exact day, a new product was added to our inventory.
Finally, we have the sales table. This table basically allows us to keep a record of which products we have sold. To do this, we use the sales ID. We request the product ID (which we obtain from the product table to know which product we are selling), the quantity (that is, how many units of that product we are selling), and, of course, the date. Just as with purchases, it is very important to include the date, as it allows us to know and maintain more rigorous control over the day and time that the sale was made.
The program initializes on a page that displays a general summary of existing inventory, grouped by category, giving us an overview of how much stock we have and the estimated value per category. At the bottom of the main page, we will find three very useful options. The first is "Sell." Clicking this allows us to create a shopping list or a kind of cart where we can add products as needed. When we click the "Sell" button, the purchase will be processed, the records in our database will be updated, and we will be returned to the main page.

The next option at the bottom is the purchase option. Here, we update the stock levels in our table; that is, registering a purchase records when we acquire new merchandise of a product already registered in our database. It will ask for the product name, the quantity, and the cost per unit. Once we register the purchase, it will redirect us to the main page, but it will display a message indicating that the purchase was successful.
We also have the inventory option. This displays a list of all the products in our database, showing their ID, name, category, price, and stock levels. If we want to search for a specific product, at the bottom of the inventory section, we find a search button. This product search page asks us to enter the product name and will search, displaying a table with all products that match or contain the character we are looking for. This is very useful because we can view specific product features without having to search directly in the general inventory.
Returning to the main page, in the form of a drop-down menu (that is, in the page header), we find four options: Sell, Search, Buy, and Add. Sell redirects us to the sales page; Search redirects us to the search section; Buy redirects us to the purchases section; and the Add button or option allows us to register new products in our database, adding new items to the products table. Important: we cannot add or complete a purchase for a product if we haven't previously added that product. So, when we want to add a new product that we haven't yet added, we access the section
To add a product, we register it and then we can make a purchase for that specific product.
To add it, we will be asked for the product name, the category it belongs to, the product description, and the unit price. It is important to note that all these fields are required; we cannot proceed or complete the registration without filling in these sections. Interestingly, in the same menu, on the left side, we find the word "Inventory" in bold. This option returns us to our main page.
#Important Aspects
• A function was implemented to identify if a database file exists. If none is detected, it will create one and load the necessary tables for the program to function. This results in a more stable system, reducing the possibility of failure due to the loss of a file.
• To provide greater protection in case of unexpected errors, all functions are enclosed within a "try except" block configured to display the error in the Python terminal and redirect you to the apology page, highlighting which function failed. This ensures program stability and improves the user experience.

#How to Run It for the First Time: 
Setting up the environment is very simple. The minimum requirements are Visual Studio Code and a Python version higher than 3.12. First, download the project documents folder. Once extracted, access the folder in Visual Studio Code. A dedicated terminal will open when you right-click on the folder, and then run the following command: "pip install -r requerimientos.txt". This will install the libraries. When the process is complete, simply run the command "flask run" and click on the link that appears in the terminal; a window will then open in your browser.
#How to Run It for the First Time:
Setting up the environment is very simple. The minimum requirements are Visual Studio Code and a Python version higher than 3.12. # Technologies Used:
• Python (Server-side logic)
• Flask (Turns the computer into a server)
• SQLite (Database support)
• CS50 SQL Library (Used for simple queries in SQLite3)
• HTML, CSS (Client-side logic)
• Bootstrap 5 (CSS library to enhance the website's style)
• Jinja2 (For reusing HTML templates)

# Structure
• Project
-Static (folder)
• Images
• Styles.css (File containing the custom styles used in this program)
- Templates(folder)
• apology.html (Template for error messages)
• buscar.html (Page that allows searching for products)
• comprar.html (File that supports the shopping interface)
• index.html (File containing the program's main page with the summary table) inventory)
• inventario.html (Page containing a detailed view of the existing inventory)
• layout.html (Template with all the data for the page header and dropdown menu)
• nuevo_producto.html (Page to register a new product)
• vender.html (Page with the shopping interface and a summary of the shopping list)
-app.py (File that supports all the server logic, with key functions and specific SQLite queries)
- helpers.py (Auxiliary file containing the logic for the apology function and table creation; this is necessary for the first use of the program)
- requerimientos.txt (File with the list of libraries used for the program's operation)
