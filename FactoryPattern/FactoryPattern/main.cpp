#include <iostream>
using namespace std;

class Notification
{
public:
	virtual void send(const string &message) = 0;
};

class EmailNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "Sending email: " << message << endl;
    }
};

class SMSNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "Sending SMS: " << message << endl;
    }
};

class NotificationCreator
{
public:
    virtual Notification* createNotification() = 0; //createobject is virtual and must be instantiated by concrete classes
    //Define an interface for creating an object, but let subclasses decide which class to instantiate.
    //Factory Method lets a class defer instantiation to subclasses.
    void send(const string& message)
    {
        Notification* notification = createNotification();
        notification->send(message);
        delete(notification);
    }
};

class EmailNotificationCreator :public NotificationCreator //Concrete classes will create object
{
public:
    Notification* createNotification() override
    {
        return new EmailNotification();
    }
};

class SMSNotificationCreator :public NotificationCreator
{
public:
    Notification* createNotification() override
    {
        return new SMSNotification();
    }
};

int main()
{
    NotificationCreator* creator;

    // Send Email
    creator = new EmailNotificationCreator();
    creator->send("Welcome to our platform!");
    delete creator;

    // Send SMS
    creator = new SMSNotificationCreator();
    creator->send("Your OTP is 123456");
    delete creator;
}