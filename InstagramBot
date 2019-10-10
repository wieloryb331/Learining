from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.keys import Keys
from selenium.common.exceptions import NoSuchElementException, StaleElementReferenceException
import time
from datetime import datetime, timedelta
import json
from random import randint


class IGBot:
    def __init__(self, username, password):
        """
        Args:
        :param username:str - used to log in
        :param password:str - used to log in

        Attributes:
        driver - webdriver used to automate things
        baseUrl:str - url used to prevent repetitions
        """
        options = webdriver.ChromeOptions()
        options.add_argument('--lang=en')
        self.driver = webdriver.Chrome("chromedriver.exe", options=options)
        self.username = username
        self.password = password
        self.baseUrl = 'https://www.instagram.com/'

    def sing_in(self):
        """
        Functionatlity:
        Logs into a specified (in class initializer) account, omits the notification button if there is one
        """
        self.driver.get('{}accounts/login/'.format(self.baseUrl))

        # waits until page is fully loaded
        wait = WebDriverWait(self.driver, 5)
        inputElement = wait.until(EC.presence_of_element_located((By.NAME, 'username')))

        self.driver.find_element_by_name('username').send_keys(self.username)
        passwordInput = self.driver.find_element_by_name('password')
        passwordInput.send_keys(self.password)
        passwordInput.send_keys(Keys.ENTER)
        time.sleep(5)

        # checks if notification button pops up, if it does then gets rid of it
        notificationButton = self.driver.find_elements_by_xpath("//button[contains(text(),'Not Now')]")
        if len(notificationButton) != 0:
            notificationButton[0].click()

    def follow_user_from_current_page(self):
        """
        Functionality:
        Clicks "Follow Button" on page that is already open. Saves username and current date to a file in json format.
        """
        followButton = self.driver.find_elements_by_xpath("//button[contains(text(),'Follow')]")
        if len(followButton) != 0:
            followButton[0].click()
            username = self.driver.find_element_by_css_selector('h1._7UhW9').text
            with open("list_of_followed.txt", 'a') as file:
                todaysDate = datetime.now().strftime("%d/%m/%Y %H:%M:%S")
                data = {'username': username, "date": todaysDate}
                json.dump(data, file)
                file.write("\n")

    def unfollow_user_from_current_page(self):
        """
        Functionality:
        unfollows user (needs to be on target's profile page). If target is not followed, does nothing.
        :return:int - returns 1 after unfollowing target and if couldn't find the correct page. If time ban occurs,
                                                                                                returns 0.
        """
        button = self.driver.find_elements_by_xpath("//button[contains(text(),'Following')]")
        if len(button) > 0:
            while 1:
                try:
                    button[0].click()
                    time.sleep(randint(4, 8))
                    break
                except StaleElementReferenceException as e:
                    print(e)
                    time.sleep(1)
            while 1:
                try:
                    popupButton = self.driver.find_elements_by_xpath("//button[contains(text(),'Unfollow')]")
                    popupButton[0].click()
                    break
                except StaleElementReferenceException as e:
                    time.sleep(1)
            return 1
        button = self.driver.find_elements_by_xpath("//button[contains(text(),'Follow Back')]")
        if len(button) > 0:
            return 1

        button = self.driver.find_elements_by_xpath("//button[contains(text(),'Follow')]")
        if len(button) > 0:
            return 1

        check = self.driver.find_elements_by_css_selector(".error-container h2")
        if len(check) > 0:
            if check[0].text == "Error":
                return 0
            else:
                return 1


    # TODO write description
    def unfollow_users_by_days(self, numberOfDays):
        usernamesToUnfollow = []
        todaysDate = datetime.now().strftime("%d/%m/%Y %H:%M:%S")
        dateXdaysAgo = datetime.strptime(todaysDate, '%d/%m/%Y %H:%M:%S') - timedelta(days=numberOfDays)

        with open("list_of_followed.txt", "r") as file:
            lines = file.readlines()
        for line in lines:
            data = json.loads(line)
            followDate = datetime.strptime(data['date'], '%d/%m/%Y %H:%M:%S')
            if followDate < dateXdaysAgo:
                usernamesToUnfollow.append(data['username'])
        counter = 0
        for username in usernamesToUnfollow:
            self.driver.get('' + self.baseUrl + username)
            time.sleep(randint(2, 4))
            counter += self.unfollow_user_from_current_page()
            time.sleep(randint(4, 8))
        if counter == len(usernamesToUnfollow):
            with open("list_of_followed.txt", "w") as file:
                for line in lines[counter::]:
                    file.write(line)
            print(f"Unfollowed {len(usernamesToUnfollow)} users that were followed before {dateXdaysAgo}")
        else:
            print(f"Something went wrong with unfollowing, unfollowed only {counter} accounts out of {len(usernamesToUnfollow)}")



    def get_photo_links_from_current_page(self, quantityOfPhotosToGather, start=0):
        """
        Args:
        :param quantityOfPhotosToGather:int - quantity of photos we want to gather
        :param start:int - number of photo we want to start from (if starting from the first one is wanted leave blank)

        Functionality:
        Gathers direct links to photos from current page

        :return:table - returns a table containing given number of links pointing directly to photos.
                        If there's only one link on the page, skips it.
        """
        photos = self.driver.find_elements_by_class_name('v1Nh3')
        links = []

        for i in range(start, len(photos)):
            link = photos[i].find_element_by_css_selector('a').get_attribute('href')
            links.append(link)

        while 1:
            if quantityOfPhotosToGather > len(links):
                self.driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
                photos = self.driver.find_elements_by_class_name('v1Nh3')
                for i in range(len(photos)):
                    try:
                        link = photos[i].find_element_by_css_selector('a').get_attribute('href')
                    except StaleElementReferenceException as e:
                        print("Caught an exception in get_photo_links_from_current_page(): " + str(e))
                        time.sleep(1)
                    if link not in links:
                        links.append(link)

            #TODO make it more universal
            elif len(photos) == 1:
                break

            else:
                break

        if len(links) > quantityOfPhotosToGather:
            while len(links) > quantityOfPhotosToGather:
                links.remove(links[-1])
        return links

    def like_photos_from_current_page(self, quantityOfPhotosToLike):
        """
        Args:
        :param quantityOfPhotosToLike:int - quantity of photos you want to like from current page

        Functionality:
        Likes given amount of photos from current page. If a photo is already liked does nothing.
        """

        links = self.get_photo_links_from_current_page(quantityOfPhotosToLike)

        for link in links:
            self.driver.get(link)
            checkLikeButton = self.driver.find_element_by_css_selector(".dCJp8 span").get_attribute('aria-label')
            if checkLikeButton == "Like":
                self.driver.find_element_by_css_selector('.dCJp8').click()
                time.sleep(18)

    def get_users_by_hashtag(self, hashtag, quantityOfUsers):
        """
        Args:
        :param hashtag:str - hashtag you want to grab photos posted with
        :param quantityOfUsers:int - defines how many users will be returned in list of users

        Functionality:
        Creates a list of users that used a particular hashtag in their photo.

        :return:table - returns a table of links to user's account that posted photos with particular hashtag
        """
        self.driver.get(''+self.baseUrl+'explore/tags/'+hashtag)
        links = self.get_photo_links_from_current_page(quantityOfUsers, 9)
        listOfUsers = []
        for link in links:
            self.driver.get(link)
            try:
                user = self.driver.find_element_by_css_selector('.FPmhX').get_attribute('href')
            except NoSuchElementException as e:
                print("Caught an exception in function get_users_by_hashtag. -> " + str(e))
                continue
            listOfUsers.append(user)
            time.sleep(2)

        return listOfUsers

    def perform_the_attack(self, hashtaglist, quantityOfUsersToAttack=100, quantityOfPicsToLikeEachUser=2, follow=False):
        """
        Args:
        :param hashtaglist:str - list of hashtags in 'puppy dog love' format
        :param quantityOfUsersToAttack:int - quantity of users to attack
        :param quantityOfPicsToLikeEachUser:int - number of photos to like on each user page
        :param follow:bool - if True than follows users, if False then doesnt follow

        Functionality:
        Performs the attack, ~200likes per hour and ~50followed people per hour
        """
        for hashtag in hashtaglist.split():
            usersToAttack = self.get_users_by_hashtag(hashtag, quantityOfUsersToAttack)
            print("Number of people gathered with #{}: {}".format(hashtag, len(usersToAttack)))
            for user in usersToAttack:
                self.driver.get(user)
                if follow:
                    self.follow_user_from_current_page()
                self.like_photos_from_current_page(quantityOfPicsToLikeEachUser)


if __name__ == "__main__":
    bot = IGBot('', '')  # insert your login and password here
    bot.sing_in()
    bot.perform_the_attack("doggo", 20, 2, follow=False)
    bot.unfollow_users_by_days(2.5)
    bot.driver.quit()
