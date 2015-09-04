import unittest
import time
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By

# Adjust hyperlinks as necessary 
def setUp(self, browser):
    # Running on Ubuntu (Firefox)
    if browser == 1:
        self.driver = webdriver.Firefox()
        #self.driver.get("http://localhost:8080/pureweb/app?client=html5&name=CartaSkeleton3&username=dan12&password=Cameron21")
        #self.driver.get("http://199.116.235.162:8080/pureweb/app/unix:1.0/2/20801/2?client=html5&name=CartaSkeleton3")
        self.driver.get("http://142.244.190.171:8080/pureweb/app/unix:0.0/4/143/1?client=html5&name=CartaSkeleton3")
        self.driver.implicitly_wait(20)

    # Running on Mac (Chrome)
    if browser == 2:
        # Change the path to where chromedriver is located
        chromedriver = "/Users/Madhatter/Downloads/chromedriver"
        self.driver = webdriver.Chrome(chromedriver)
        #self.driver.get("http://199.116.235.162:8080/pureweb/app/unix:1.0/2/20801/2?client=html5&name=CartaSkeleton3")
        self.driver.get("http://142.244.190.171:8080/pureweb/app/unix:0.0/4/143/1?client=html5&name=CartaSkeleton3")
        self.driver.implicitly_wait(20)
        
# Clear text box and change value of the text box
# Selenium clear() method is sometimes unable to clear 
# default text, therefore a manual method has been implemented
def _changeElementText(self, driver, elementText, inputValue):
    # First click on the element and get the element value
    ActionChains(driver).move_to_element( elementText ).click( elementText ).perform()
    elementValue = elementText.get_attribute("value")
    # Clear the content in the text box
    for x in range(0, len(str(elementValue))):
        elementText.send_keys( Keys.BACK_SPACE )
    # Enter a new value into the text box and wait for element to update value
    # otherwise Google Chrome browser will not receive the correct value
    elementText.send_keys( inputValue )
    elementText.send_keys( Keys.ENTER )
    # Get the new element value
    elementValue = elementText.get_attribute("value")
    return elementValue

def animation_to_image_window(unittest, driver):
    animWindow = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.XPATH, "//div[@qxclass='skel.widgets.Window.DisplayWindowAnimation']")))
    unittest.assertIsNotNone( animWindow, "Could not find animation window")
    ActionChains(driver).click( animWindow ).perform()
        
    # Make sure the animation window is enabled by clicking an element within the window
    # Chrome browser is unable to enable the animation window by clicking on the window
    channelText = WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.ID, "ChannelIndexText")))
    ActionChains(driver).click( channelText).perform()
    ActionChains(driver).context_click( channelText ).send_keys(Keys.ARROW_RIGHT).send_keys(Keys.ARROW_RIGHT).send_keys(Keys.ENTER).perform()

def get_window_count(unittest, driver):
     windowList = driver.find_elements_by_xpath("//div[@qxclass='qx.ui.window.Desktop']")
     windowCount = len( windowList )
     return windowCount
 
#Set a custom layout with the given number of rows and columns
def layout_custom(self, driver, rows, cols ):

    # Wait for the image window to be present (ensures browser is fully loaded)
    imageWindow = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.XPATH, "//div[@qxclass='skel.widgets.Window.DisplayWindowImage']")))
    imageWindow = WebDriverWait(driver, 20).until(EC.visibility_of_element_located((By.XPATH, "//div[@qxclass='skel.widgets.Window.DisplayWindowImage']")))
    ActionChains(driver).click( imageWindow )

    # Find the layout button on the menu bar and click it.
    self._clickLayoutButton( driver )
    
    # Find the layout custom button in the submenu and click it.
    customLayoutButton = WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.XPATH, "//div[text()='Custom Layout']")))
    self.assertIsNotNone( customLayoutButton, "Could not find custom layout button in submenu")
    ActionChains(driver).click( customLayoutButton ).perform()

    # Get the row count spin and set its value.
    rowSpin = WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.XPATH, "//div[starts-with(@id,'customLayoutRows')]/input")))
    self.assertIsNotNone( rowSpin, "Could not find custom layout row indicator")
    rowSpin.send_keys( Keys.BACK_SPACE )
    rowSpin.send_keys( str(rows) )
    
    # Get the column count spin and set its value.
    colSpin = WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.XPATH, "//div[starts-with(@id,'customLayoutCols')]/input")))
    self.assertIsNotNone( colSpin, "Could not find custom layout column indicator")
    colSpin.send_keys( str(cols) )
    colSpin.send_keys( Keys.ARROW_LEFT )
    colSpin.send_keys( Keys.BACK_SPACE )
    
    # Close the custom layout dialog
    closeButton = WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.XPATH, "//div[starts-with(@id,'customLayoutOK')]")))
    self.assertIsNotNone( closeButton, "Could not find close button")
    ActionChains(driver).click(closeButton).perform()

# Load an image
def load_image(unittest, driver, imageName): 
    # If image is not specified, load default image
    # Change this to a test image available where the tests are running
    # Test image will ideally have more than 3 channels for a successful test run
    if imageName == "Default":
        imageName = "N15693D.fits"

    # Wait 20 seconds for the imageWindow to appear on the page
    imageWindow = WebDriverWait(driver, 30).until(EC.presence_of_element_located((By.XPATH, "//div[@qxclass='skel.widgets.Window.DisplayWindowImage']")))

    # Find a window capable of loading an image and select the window
    ActionChains(driver).click( imageWindow ).perform()
    # Precaution, sometimes the driver does not enable the window when first clicks the window
    ActionChains(driver).click( imageWindow ).perform()

    # Click the data button
    dataButton = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.XPATH, "//div[text()='Data']/..")))
    unittest.assertIsNotNone( dataButton, "Could not click data button in the context menu")
    ActionChains(driver).click(dataButton).perform()
    
    # Look for the open button and click it to open the file dialog.
    openDataButton = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.XPATH, "//div/div[text()='Open...']/..")))
    unittest.assertIsNotNone(openDataButton, "Could not click open button on data subcontext menu.")
    ActionChains(driver).send_keys(Keys.ARROW_DOWN).send_keys(Keys.ENTER).perform()

    # Select the specific image
    loadButton = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.ID, "loadFileButton")))
    fileDiv = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.XPATH, "//div[text()='"+imageName+"']")))
    driver.execute_script( "arguments[0].scrollIntoView(true);", fileDiv)
    fileDiv.click()
    
    # Click the load button
    loadButton = driver.find_element_by_id( "loadFileButton")
    unittest.assertIsNotNone(loadButton, "Could not find load button to click")
    loadButton.click()
    
    # Now close the file dialog
    closeButton = driver.find_element_by_id( "closeFileLoadButton")
    unittest.assertIsNotNone(loadButton, "Could not find button to close the file browser")
    closeButton.click()
    
    # Check that the window is displaying an image.
    viewElement = driver.find_element_by_xpath("//div[@qxclass='skel.boundWidgets.View.View']")
    unittest.assertIsNotNone(viewElement, "Could not find view element on page.")
    imageElement = driver.find_element_by_id("pwUID0")
    unittest.assertIsNotNone(imageElement, "Could not find image on the page")

    return imageWindow

# Adds a window below the main image viewer
# Return the window for future actions
def add_window(unittest, driver):
    # Click the Window button
    windowButton = WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.XPATH, "//div[@qxclass='qx.ui.toolbar.MenuButton']/div[text()='Window']/..")))
    unittest.assertIsNotNone( windowButton, "Could not find window button in the context menu")
    ActionChains(driver).click(windowButton).perform()

    # Look for the add button in the submenu.
    addButton = WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.XPATH, "//div/div[text()='Add']/..")))
    unittest.assertIsNotNone( addButton, "Could not click minimize button on window subcontext menu.")
    ActionChains(driver).click( addButton ).send_keys(Keys.ARROW_RIGHT).send_keys(Keys.ENTER).perform()

    # Check that we now have a generic empty window in the display and that the window count has gone up by one.
    emptyWindow = WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.XPATH, "//div[@qxclass='skel.widgets.Window.DisplayWindowGenericPlugin']")))
    unittest.assertIsNotNone( emptyWindow, "Could not find empty display window")
    ActionChains(driver).click( emptyWindow ).perform()

    return emptyWindow

# Add a window, convert the window to an image window
# Use the new image window to load an image
def load_image_different_window(unittest, driver, imageName):
    # Find a window capable of loading an image.
    imageWindow = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.XPATH, "//div[@qxclass='skel.widgets.Window.DisplayWindowImage']")))
    unittest.assertIsNotNone( imageWindow, "Could not find image display window")
    ActionChains(driver).click(imageWindow).perform()
    
    # Add a new window below the main casa image window
    emptyWindow = add_window( unittest, driver)
    
    # Change the plugin of the empty window to an image loader 
    ActionChains(driver).context_click( emptyWindow ).send_keys(Keys.ARROW_DOWN).send_keys(Keys.ARROW_RIGHT).send_keys(Keys.ENTER).perform()

    # Ensure that there is a new image window 
    imageWindow2 = WebDriverWait(driver, 30).until(EC.presence_of_element_located((By.ID, "CasaImageLoader2")))
    unittest.assertIsNotNone( imageWindow2, "Could not find second image window")
    ActionChains(driver).click( imageWindow2 ).perform()
    # Precaution, sometimes the driver does not enable the window when first clicks the window
    ActionChains(driver).click( imageWindow2 ).perform()

    # Click the data button
    dataButton = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.XPATH, "//div[text()='Data']/..")))
    unittest.assertIsNotNone( dataButton, "Could not click data button in the context menu")
    ActionChains(driver).click(dataButton).perform()
    
    # Look for the open button and click it to open the file dialog.
    openDataButton = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.XPATH, "//div/div[text()='Open...']/..")))
    unittest.assertIsNotNone(openDataButton, "Could not click open button on data subcontext menu.")
    ActionChains(driver).send_keys(Keys.ARROW_DOWN).send_keys(Keys.ENTER).perform()

    # Select the specific image
    loadButton = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.ID, "loadFileButton")))
    fileDiv = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.XPATH, "//div[text()='"+imageName+"']")))
    driver.execute_script( "arguments[0].scrollIntoView(true);", fileDiv)
    fileDiv.click()
    
    # Click the load button
    loadButton = driver.find_element_by_id( "loadFileButton")
    unittest.assertIsNotNone(loadButton, "Could not find load button to click")
    loadButton.click()
    
    # Now close the file dialog
    closeButton = driver.find_element_by_id( "closeFileLoadButton")
    unittest.assertIsNotNone(loadButton, "Could not find button to close the file browser")
    closeButton.click()
    
    # Check that the window is displaying an image.
    viewElement = driver.find_element_by_xpath("//div[@qxclass='skel.boundWidgets.View.View']")
    unittest.assertIsNotNone(viewElement, "Could not find view element on page.")
    imageElement = driver.find_element_by_id("pwUID0")
    unittest.assertIsNotNone(imageElement, "Could not find image on the page")

    # Return the second image loader window for further linking tests
    return imageWindow2