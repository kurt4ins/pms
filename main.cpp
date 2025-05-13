#include <iostream>
#include <string>
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
    
    // ===== Бронирование номеров =====
    printHeader("БРОНИРОВАНИЕ НОМЕРОВ");
    
    // Создаем объекты бронирования
    Booking* booking1 = new Booking();
    Booking* booking2 = new Booking();
    
    // Добавляем комнаты в систему бронирования
    booking1->addRoom(standardRoom);
    booking1->addRoom(businessRoom);
    booking1->addRoom(luxuryRoom);
    
    booking2->addRoom(standardRoom);
    booking2->addRoom(businessRoom);
    booking2->addRoom(luxuryRoom);
    
    // Добавляем ресурсы в систему бронирования
    booking1->addResource(spa);
    booking1->addResource(gym);
    booking1->addResource(restaurant);
    
    booking2->addResource(spa);
    booking2->addResource(gym);
    booking2->addResource(restaurant);
    
    // Устанавливаем даты заезда и выезда
    time_t now = time(nullptr);
    
    // Бронирование для Иванова: заезд сегодня, выезд через 3 дня
    time_t checkInIvanov = now;
    time_t checkOutIvanov = now + (3 * 24 * 60 * 60);
    
    // Бронирование для Петровой: заезд через 5 дней, выезд через 10 дней
    time_t checkInPetrova = now + (5 * 24 * 60 * 60);
    time_t checkOutPetrova = now + (10 * 24 * 60 * 60);
    
    // Создаем бронирования
    u_int64_t reservationIvanov = booking1->createReservation(
        ivanov->getId(), standardRoom->getRoomNumber(), checkInIvanov, checkOutIvanov);
    
    u_int64_t reservationPetrova = booking2->createReservation(
        petrova->getId(), businessRoom->getRoomNumber(), checkInPetrova, checkOutPetrova);
    
    std::cout << "Созданы бронирования:" << std::endl;
    std::cout << "- Бронирование для " << ivanov->getFullName() << ": " << std::endl
              << "  ID: " << reservationIvanov << std::endl
              << "  Комната: " << standardRoom->getRoomNumber() << " (" << standardRoom->getRoomType() << ")" << std::endl
              << "  Дата заезда: " << getTimeString(checkInIvanov) << std::endl
              << "  Дата выезда: " << getTimeString(checkOutIvanov) << std::endl
              << "  Статус: " << booking1->getBookingStatus(reservationIvanov) << " (0=RESERVED, 1=CHECKED_IN)" << std::endl;
              
    std::cout << "\n- Бронирование для " << petrova->getFullName() << ": " << std::endl
              << "  ID: " << reservationPetrova << std::endl
              << "  Комната: " << businessRoom->getRoomNumber() << " (" << businessRoom->getRoomType() << ")" << std::endl
              << "  Дата заезда: " << getTimeString(checkInPetrova) << std::endl
              << "  Дата выезда: " << getTimeString(checkOutPetrova) << std::endl
              << "  Статус: " << booking2->getBookingStatus(reservationPetrova) << " (0=RESERVED, 1=CHECKED_IN)" << std::endl;
    
    // Проверка доступности номеров
    std::cout << "\nДоступные номера на текущие даты:" << std::endl;
    std::vector<Room*> availableRooms = booking1->getAvailableRooms(now, now + (24 * 60 * 60));
    for (size_t i = 0; i < availableRooms.size(); i++) {
        std::cout << "- Комната " << availableRooms[i]->getRoomNumber() 
                  << ": " << availableRooms[i]->getRoomType() << std::endl;
    }
    
    // ===== Заселение гостя =====
    printHeader("ЗАСЕЛЕНИЕ ГОСТЯ");
    
    if (booking1->checkIn(reservationIvanov)) {
        std::cout << "Гость " << ivanov->getFullName() << " успешно заселен в комнату " 
                  << standardRoom->getRoomNumber() << " (" << standardRoom->getRoomType() << ")" << std::endl;
        
        // Записываем события взаимодействия с гостем (в визит-историю)
        // В текущем визите гостя добавляем записи о взаимодействиях
        std::cout << "\nЗаписываем взаимодействия с гостем:" << std::endl;
        
        // Создаем записи о взаимодействиях в истории посещений (подход отличается от оригинального)
        std::cout << "- Выдан ключ от номера" << std::endl;
        std::cout << "- Объяснены правила проживания в отеле" << std::endl;
        std::cout << "- Выдана карта города и список достопримечательностей" << std::endl;
        
        // Проверка статуса бронирования
        std::cout << "\nНовый статус бронирования: " 
                  << booking1->getBookingStatus(reservationIvanov) 
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
    
    u_int64_t spaBookingId = booking1->bookResource(spa->getId(), spaStartTime, spaEndTime);
    u_int64_t gymBookingId = booking1->bookResource(gym->getId(), gymStartTime, gymEndTime);
    
    std::cout << "Забронированы услуги для " << ivanov->getFullName() << ":" << std::endl;
    std::cout << "- " << spa->getName() << ": " << getTimeString(spaStartTime) 
              << " (ID бронирования: " << spaBookingId << ")" << std::endl;
    std::cout << "- " << gym->getName() << ": " << getTimeString(gymStartTime) 
              << " (ID бронирования: " << gymBookingId << ")" << std::endl;
    
    // Проверяем доступные услуги на завтра
    time_t tomorrowStart = now + (24 * 60 * 60);
    time_t tomorrowEnd = tomorrowStart + (24 * 60 * 60);
    
    std::cout << "\nДоступные услуги на завтра:" << std::endl;
    std::vector<Resource*> availableResources = booking1->getAvailableResources(tomorrowStart, tomorrowEnd);
    for (size_t i = 0; i < availableResources.size(); i++) {
        std::cout << "- " << availableResources[i]->getName() << std::endl;
    }
    
    // ===== Счет за проживание =====
    printHeader("СЧЕТ ЗА ПРОЖИВАНИЕ");
    
    // При заселении автоматически создается счет
    std::vector<Bill*> allBills = booking1->getAllBills();
    
    if (!allBills.empty()) {
        u_int64_t billId = allBills[0]->getBillId();
        
        // Добавляем бронирования услуг в счет
        booking1->addResourceToBill(billId, spa->getId(), spaBookingId);
        booking1->addResourceToBill(billId, gym->getId(), gymBookingId);
        
        // Добавляем дополнительные услуги
        booking1->addBillItem(billId, "Мини-бар", 1200.0);
        booking1->addBillItem(billId, "Завтрак", 800.0);
        
        // Получаем и выводим счет
        std::cout << "Счет для гостя " << ivanov->getFullName() << ":" << std::endl;
        std::cout << booking1->generateBillSummary(billId) << std::endl;
        
        // Частичная оплата счета
        double totalAmount = booking1->getTotalBillAmount(billId);
        booking1->payBillAmount(billId, totalAmount / 2);
        
        std::cout << "\nВнесена частичная оплата. Остаток к оплате: " 
                  << booking1->getRemainingBillAmount(billId) << " руб." << std::endl;
    } else {
        std::cout << "Счет не найден!" << std::endl;
    }
    
    // ===== Выселение гостя =====
    printHeader("ВЫСЕЛЕНИЕ ГОСТЯ");
    
    // Сначала полностью оплачиваем счет
    if (!allBills.empty()) {
        u_int64_t billId = allBills[0]->getBillId();
        booking1->payBill(billId);
        std::cout << "Счет полностью оплачен. Остаток: " 
                  << booking1->getRemainingBillAmount(billId) << " руб." << std::endl;
    }
    
    // Теперь выселяем гостя
    if (booking1->checkOut(reservationIvanov)) {
        std::cout << "\nГость " << ivanov->getFullName() << " успешно выселен из комнаты " 
                  << standardRoom->getRoomNumber() << std::endl;
        
        // Проверка статуса бронирования
        std::cout << "Новый статус бронирования: " 
                  << booking1->getBookingStatus(reservationIvanov) 
                  << " (2=CHECKED_OUT)" << std::endl;
                  
        // Проверка начисления бонусных баллов
        std::cout << "\nБонусные баллы гостя после выселения: " 
                  << ivanov->getLoyalPoints() << std::endl;
    } else {
        std::cout << "Не удалось выселить гостя " << ivanov->getFullName() 
                  << ". Возможно, не все счета оплачены." << std::endl;
    }
    
    // Освобождаем память
    delete standardRoom;
    delete businessRoom;
    delete luxuryRoom;
    delete spa;
    delete gym;
    delete restaurant;
    delete ivanov;
    delete petrova;
    delete booking1;
    delete booking2;
    
    return 0;
}
