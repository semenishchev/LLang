// Example file how I see the language

interface Human {}
interface Obj {
    @Getter("checker") @Setter boolean checkLiving(int age)
}

@SomeMarker
class Developer impl Human, Obj {
    @Getter final String name
    @Getter("phoneNumber") @Setter String phoneNumber = null;
    public Developer(String name) {
        this.name = name
        println("Address of name is ${&name}")
    }
}

interface Living {
    void talk();
}

impl Living with Developer // will force Developer to implement Living interface

void Developer::talk() {
    println("My name is ${this.name}")
}

void main() {
    String[] areas = ["web", "game", "native"]
    for area in areas {
        println("Hello, $area developers")
    }

    Developer developer = new Developer("Sasha")

    developer.setName("") // error
    developer.setPhoneNumber("+1111111111")
    developer.setChecker((age) -> {
        if age < 0 { return false }
        return age < 100
    })
    developer.checker(123) // will actually call user-defined method with `123` as args
    println(developer.getName == "Sasha" ? "This program runs by the developer of the language" : "User-code")
    println(developer.getName() + " ${developer.phoneNumber()}")

    // if we don't call `delete developer`, compiler will give us an error, because all objects must be freed, but where - it's developer's decision
    delete developer
}