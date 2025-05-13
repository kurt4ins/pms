#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include "includes/Booking.hpp"
#include "includes/Room.hpp"
#include "includes/Guest.hpp"
#include "includes/Resource.hpp"
#include "includes/DateTime.hpp"

void printLine() {
    std::cout << std::string(60, '-') << std::endl;
}

void printHeader(const std::string& header) {
    printLine();
    std::cout << " " << header << std::endl;
    printLine();
}

std::string getTimeString(time_t time) {
    return DateTime(time).dateToStr();
}

int main() {
    printHeader("ИНИЦИАЛИЗАЦИЯ ОТЕЛЯ");
    
    // Создаем комнаты разных типов
    Room* standardRoom = new Room(101, "Стандарт");
    Room* businessRoom = new Room(201, "Бизнес");
    Room* luxuryRoom = new Room(301, "Люкс");
    
    std::cout << "Создано 3 комнаты:" << std::endl;
    std::cout << "- Комната " << standardRoom->getRoomNumber() << ": " << standardRoom->getRoomType() << std::endl;
    std::cout << "- Комната " << businessRoom->getRoomNumber() << ": " << businessRoom->getRoomType() << std::endl;
    std::cout << "- Комната " << luxuryRoom->getRoomNumber() << ": " << luxuryRoom->getRoomType() << std::endl;
    
    // Создаем услуги
    Resource* spa = new Resource("СПА", SPA, 2000.0);
    Resource* gym = new Resource("Фитнес-центр", GYM, 500.0);
    Resource* restaurant = new Resource("Ресторан", OTHER, 1500.0);
    
    std::cout << "\nСозданы услуги:" << std::endl;
    std::cout << "- " << spa->getName() << " (" << spa->getRate() << " руб./час)" << std::endl;
    std::cout << "- " << gym->getName() << " (" << gym->getRate() << " руб./час)" << std::endl;
    std::cout << "- " << restaurant->getName() << " (" << restaurant->getRate() << " руб./час)" << std::endl;
    
    // Создаем гостей
    Guest* ivanov = new Guest(1001, "Иван", "Иванов", "+7-900-111-2233", 
                              "ivanov@mail.ru", 4510123456, "15-05-1985", 100);
    Guest* petrova = new Guest(1002, "Елена", "Петрова", "+7-900-222-3344", 
                               "petrova@mail.ru", 4510234567, "23-07-1990", 50);
    
    std::cout << "\nСозданы гости:" << std::endl;
    std::cout << "- " << ivanov->getFullName() << " (ID: " << ivanov->getId() << ")" << std::endl;
    std::cout << "- " << petrova->getFullName() << " (ID: " << petrova->getId() << ")" << std::endl;
    
    // Регистрируем гостей в системе комнат
    standardRoom->addGuest(ivanov);
    businessRoom->addGuest(petrova);
    

    printHeader("СИСТЕМА БРОНИРОВАНИЯ");
    
    // Создаем систему бронирования
    Booking* bookingSystem = new Booking();
    
    // Добавляем комнаты в систему бронирования
    bookingSystem->addRoom(standardRoom);
    bookingSystem->addRoom(businessRoom);
    bookingSystem->addRoom(luxuryRoom);
    
    // Добавляем ресурсы в систему бронирования
    bookingSystem->addResource(spa);
    bookingSystem->addResource(gym);
    bookingSystem->addResource(restaurant);
    
    std::cout << "Создана единая система управления отелем" << std::endl;
    

    printHeader("БРОНИРОВАНИЕ НОМЕРОВ");
    
    // Устанавливаем даты заезда и выезда
    time_t now = time(nullptr);
    
    // Бронирование для Иванова: заезд сегодня, выезд через 3 дня
    time_t checkInIvanov = now;
    time_t checkOutIvanov = now + (3 * 24 * 60 * 60);
    
    // Бронирование для Петровой: заезд через 5 дней, выезд через 10 дней
    time_t checkInPetrova = now + (5 * 24 * 60 * 60);
    time_t checkOutPetrova = now + (10 * 24 * 60 * 60);
    
    // Создаем бронирования
    u_int64_t reservationIvanov = bookingSystem->createReservation(ivanov->getId(), standardRoom->getRoomNumber(), checkInIvanov, checkOutIvanov);
    
    u_int64_t reservationPetrova = bookingSystem->createReservation(petrova->getId(), businessRoom->getRoomNumber(), checkInPetrova, checkOutPetrova);
    
    std::cout << "Созданы бронирования:" << std::endl;
    std::cout << "- Бронирование для " << ivanov->getFullName() << ": " << std::endl
              << "  ID: " << reservationIvanov << std::endl
              << "  Комната: " << standardRoom->getRoomNumber() << " (" << standardRoom->getRoomType() << ")" << std::endl
              << "  Дата заезда: " << getTimeString(checkInIvanov) << std::endl
              << "  Дата выезда: " << getTimeString(checkOutIvanov) << std::endl
              << "  Статус: " << bookingSystem->getBookingStatus(reservationIvanov) << " (0=RESERVED, 1=CHECKED_IN)" << std::endl;
              
    std::cout << "\n- Бронирование для " << petrova->getFullName() << ": " << std::endl
              << "  ID: " << reservationPetrova << std::endl
              << "  Комната: " << businessRoom->getRoomNumber() << " (" << businessRoom->getRoomType() << ")" << std::endl
              << "  Дата заезда: " << getTimeString(checkInPetrova) << std::endl
              << "  Дата выезда: " << getTimeString(checkOutPetrova) << std::endl
              << "  Статус: " << bookingSystem->getBookingStatus(reservationPetrova) << " (0=RESERVED, 1=CHECKED_IN)" << std::endl;
    
    // Проверка доступности номеров
    std::cout << "\nДоступные номера на текущие даты (" << getTimeString(now) << " — " << getTimeString(now + (24 * 60 * 60)) << "):" << std::endl;
    std::vector<Room*> availableRooms = bookingSystem->getAvailableRooms(now, now + (24 * 60 * 60));
    for (size_t i = 0; i < availableRooms.size(); i++) {
        std::cout << "- Комната " << availableRooms[i]->getRoomNumber() 
                  << ": " << availableRooms[i]->getRoomType() << std::endl;
    }
    

    printHeader("ЗАСЕЛЕНИЕ ГОСТЯ");
    
    if (bookingSystem->checkIn(reservationIvanov)) {
        std::cout << "Гость " << ivanov->getFullName() << " успешно заселен в комнату " 
                  << standardRoom->getRoomNumber() << " (" << standardRoom->getRoomType() << ")" << std::endl;
        
    
        
        // Проверка статуса бронирования
        std::cout << "\nНовый статус бронирования: " 
                  << bookingSystem->getBookingStatus(reservationIvanov) 
                  << " (1=CHECKED_IN)" << std::endl;
    } else {
        std::cout << "Не удалось заселить гостя " << ivanov->getFullName() << std::endl;
    }
    
    // ===== Бронирование услуг =====
    printHeader("БРОНИРОВАНИЕ УСЛУГ");
    
    // Бронируем СПА на завтра на 2 часа
    time_t spaStartTime = now + (24 * 60 * 60);
    time_t spaEndTime = spaStartTime + (2 * 60 * 60);
    
    // Бронируем фитнес через 2 дня на 1.5 часа
    time_t gymStartTime = now + (2 * 24 * 60 * 60);
    time_t gymEndTime = gymStartTime + (90 * 60);
    
    u_int64_t spaBookingId = bookingSystem->bookResource(reservationIvanov, spa->getId(), spaStartTime, spaEndTime);
    u_int64_t gymBookingId = bookingSystem->bookResource(reservationIvanov, gym->getId(), gymStartTime, gymEndTime);
    
    std::cout << "Забронированы услуги для " << ivanov->getFullName() << ":" << std::endl;
    std::cout << "- " << spa->getName() << ": " << getTimeString(spaStartTime) << " (ID бронирования: " << spaBookingId << ")" << std::endl;
    std::cout << "- " << gym->getName() << ": " << getTimeString(gymStartTime) << " (ID бронирования: " << gymBookingId << ")" << std::endl;


    printHeader("СЧЕТ ЗА ПРОЖИВАНИЕ");
    
    // Получаем счета для бронирования Иванова
    std::vector<Bill*> ivanovBills = bookingSystem->getBookingBills(reservationIvanov);
    
    if (!ivanovBills.empty()) {
        u_int64_t billId = ivanovBills[0]->getBillId();
        
        // Добавляем бронирования услуг в счет
        bookingSystem->addResourceToBill(billId, spa->getId(), spaBookingId);
        bookingSystem->addResourceToBill(billId, gym->getId(), gymBookingId);
        
        // Добавляем дополнительные услуги
        bookingSystem->addBillItem(billId, "Мини-бар", 1200.0);
        bookingSystem->addBillItem(billId, "Завтрак", 800.0);
        
        // Получаем и выводим счет
        std::cout << "Счет для гостя " << ivanov->getFullName() << ":" << std::endl;
        std::cout << bookingSystem->generateBillSummary(billId) << std::endl;
        
        // Частичная оплата счета
        double totalAmount = bookingSystem->getTotalBillAmount(billId);
        bookingSystem->payBillAmount(billId, totalAmount / 2);
        
        std::cout << "\nВнесена частичная оплата. Остаток к оплате: " << bookingSystem->getRemainingBillAmount(billId) << " руб." << std::endl;
    } else {
        std::cout << "Счет не найден!" << std::endl;
    }
    

    printHeader("ВЫСЕЛЕНИЕ ГОСТЯ");
    
    // Сначала полностью оплачиваем счет
    if (!ivanovBills.empty()) {
        u_int64_t billId = ivanovBills[0]->getBillId();
        bookingSystem->payBill(billId);
        std::cout << "Счет полностью оплачен. Остаток: " << bookingSystem->getRemainingBillAmount(billId) << " руб." << std::endl;
    }
    
    // Теперь выселяем гостя
    if (bookingSystem->checkOut(reservationIvanov)) {
        std::cout << "\nГость " << ivanov->getFullName() << " успешно выселен из комнаты " << standardRoom->getRoomNumber() << std::endl;
        
        // Проверка статуса бронирования
        std::cout << "Новый статус бронирования: " << bookingSystem->getBookingStatus(reservationIvanov) << " (2=CHECKED_OUT)" << std::endl;
                  
        // Проверка начисления бонусных баллов
        std::cout << "\nБонусные баллы гостя после выселения: " << ivanov->getLoyalPoints() << std::endl;
    } else {
        std::cout << "Не удалось выселить гостя " << ivanov->getFullName() << ". Возможно, не все счета оплачены." << std::endl;
    }
    

    printHeader("СТАТИСТИКА СИСТЕМЫ");
    
    std::cout << "Всего бронирований в системе: " << bookingSystem->getAllBookings().size() << std::endl;
    
    std::vector<u_int64_t> petrovaBookings = bookingSystem->getGuestBookings(petrova->getId());
    std::cout << "Количество бронирований для " << petrova->getFullName() << ": " << petrovaBookings.size() << std::endl;
    
    // Освобождаем память
    delete standardRoom;
    delete businessRoom;
    delete luxuryRoom;
    delete spa;
    delete gym;
    delete restaurant;
    delete ivanov;
    delete petrova;
    delete bookingSystem;
    
    return 0;
}
