from selenium import webdriver
from selenium.webdriver import Chrome
from selenium.webdriver.common import keys
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

from random import randint

driver = webdriver.Chrome()
driver.get("https://localhost/PrestaShop/")

category_dzisiaj = driver.find_element(By.LINK_TEXT, "Dzisiaj").get_property("href")
category_familijny = driver.find_element(By.LINK_TEXT, "Familijny").get_property("href")

def getMovies():
	movies = driver.find_elements(By.CSS_SELECTOR, "a.product-thumbnail")
	movie_links = [None] * len(movies)

	for x in range(len(movies)):
		movie_links[x] = movies[x].get_property("href")
	return movie_links


def addToCart(driver, number):
	numeric_input = driver.find_element(By.NAME, "qty")
	numeric_input.send_keys(Keys.DELETE)
	numeric_input.send_keys(number)
	
	button = driver.find_element(By.CSS_SELECTOR, "button.add-to-cart")
	button.click()

def fillCart(driver, movie_links):
	for x in range(len(movie_links)):
		#75% chance of adding to cart
		random_n = randint(0, 4)
		if random_n == 0:
			continue

		driver.get(movie_links[x])
		addToCart(driver, randint(1, 4))

#Adding products from "Dzisiaj"
driver.get(category_dzisiaj)
movie_links = getMovies()
fillCart(driver, movie_links)

#Adding products from "Familijny"
driver.get(category_familijny)
movie_links = getMovies()
fillCart(driver, movie_links)


#Go to cart
driver.get(category_familijny)
try:
	cart_button = driver.find_element(By.XPATH, '/html/body/main/header/nav/div/div/div[1]/div[2]/div[2]/div/div/a')
except:
	cart_button = driver.find_element(By.XPATH, '/html/body/main/header/nav/div/div/div[2]/div[2]/div/div/a/i')
cart_button.click()

#Remove random item from cart
items = driver.find_elements(By.CSS_SELECTOR, "a.remove-from-cart")
items[randint(0, len(items))].click()

#New account
driver.find_element(By.CSS_SELECTOR, ".checkout").click()
driver.find_element(By.NAME, "id_gender").click()
driver.find_element(By.NAME, "firstname").send_keys("Cinema")
driver.find_element(By.NAME, "lastname").send_keys("Client")
email = "client" + str(randint(1,1000)) + "@spam.pl"
driver.find_element(By.NAME, "email").send_keys(email)
driver.find_element(By.NAME, "password").send_keys("hunter1")
driver.find_element(By.NAME, "birthday").send_keys("1939-11-09")
driver.find_element(By.NAME, "optin").click()
driver.find_element(By.NAME, "customer_privacy").click()
driver.find_element(By.NAME, "psgdpr").click()
driver.find_element(By.NAME, "continue").click()
driver.find_element(By.NAME, "address1").send_keys("ul. Długa 21")
driver.find_element(By.NAME, "postcode").send_keys("42-007")
driver.find_element(By.NAME, "city").send_keys("Słowo-owo")
driver.find_element(By.NAME, "confirm-addresses").click()
driver.find_element(By.NAME, "confirmDeliveryOption").click()
driver.find_element(By.NAME, "payment-option").click()
driver.find_element(By.NAME, "conditions_to_approve[terms-and-conditions]").click()
driver.find_element(By.CSS_SELECTOR, "#payment-confirmation button").click()

#Check status
driver.find_element(By.LINK_TEXT, "Zamówienia").click()
driver.find_element(By.LINK_TEXT, "Szczegóły").click()

driver.close()