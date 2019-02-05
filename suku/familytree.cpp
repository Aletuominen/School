/* Familytree.cpp sisältää familytree-luokan metodit. Julkisen rajapinnan meto-
 * dit rakentavat luokan, ja hakevat sieltä haluttua tietoa. Yksityisen raja-
 * pinnan metodit toimivat apufunktioina julkisen rajapinnan metodeille.
 *
 * Tekijä: Aleksi Tuominen
*/
#include <familytree.hh>

// Varsinainen rakentaja on tyhjä, tietorakenteen muodostaminen toteutetaan
// metodeilla.

Familytree::Familytree()
{

}

// Luokalle ei tarvitse toteuttaa erikseen purkajaa, koska käytetään älykkäitä
// osoittimia

Familytree::~Familytree()
{

}

// Tietorakenne data_ rakenetaan lisäämällä sinne shared-pointereita
// Person-structista. Hakuavaimena data_ -mapissa toimii henkilön id.
// Jos henkilö on jo olemassa, henkilöä ei lisätä uudestaan. Vanhemmuus-
// ja lapsuussuhteet lisätään myöhemmin.

void Familytree::addNewPerson(const std::__cxx11::string &id,
                              const int &height, std::ostream &output)
{
    Personmap::const_iterator etsi = data_.find(id);
    if (etsi != data_.end()){
        output << "Error. Person already added." << std::endl;
        return;
    }

    std::shared_ptr<Person> uusi_ptr(new Person);
    uusi_ptr->id_ = id;
    uusi_ptr->height_ = height;
    data_.insert({id, uusi_ptr});

}

// Tulostaa kaikki data_ -tietorakenteen henkilöt ja heidän pituutensa.

void Familytree::printPersons(std::ostream &output) const

{
    for (Personmap::const_iterator it = data_.begin(); it != data_.end(); ++it){

        output << it->first << ", " << it->second->height_ << std::endl;
    }
}

// Lisätään lapsi-Personille vanhemmat ja vastaavasti parens-Personeille lapsi.
// Tulostetaan virheilmoitus, jos halutun nimistä lasta ei löydy. Jos vanhempien
// nimiä vastaavia henkilöitä ei löydy, ei tehdä mitään.

void Familytree::addRelation(const std::__cxx11::string &child,
                             const std::vector<std::__cxx11::string> &parents,
                             std::ostream &output)
{
    Person* child_ptr = getPointer(child);
    Person* parent_ptr = nullptr;
    int i = 0;

    if(child_ptr == nullptr){
        output << "Error. " << child <<" not found." << std::endl;
        return;
    }
    for (std::string id : parents){

        parent_ptr = getPointer(id);

        if(parent_ptr == nullptr){
            continue;
        }

        parent_ptr->children_.push_back(child_ptr);
        child_ptr->parents_[i++] = parent_ptr;
    }
}

// Tulostetaan yhden vanhemman kaikki lapset apufunktioita hyödyntäen.
// Tulostetaan virheilmoitus, jos henkilöä ei löydy.

void Familytree::printChildren(const std::__cxx11::string &id,
                               std::ostream &output) const
{

    Person* parent_ptr = getPointer(id);

    if (parent_ptr == nullptr){
        output << "Error. " << id << " not found." << std::endl;
        return;
    }
    IdSet children_set = VectorToIdSet(parent_ptr->children_);

    printGroup(id, "children", children_set, output);

}

// Tulostetaan yhden lapsen kaikki vanhemmat apufunktioita hyödyntäen.
// Tulostetaan virheilmoitus, jos henkilöä ei löydy.

void Familytree::printParents(const std::__cxx11::string &id,
                              std::ostream &output) const
{

    Person* child_ptr = getPointer(id);

    if (child_ptr == nullptr){
        output << "Error. " << id << " not found." << std::endl;
        return;
    }

    IdSet parents_set = VectorToIdSet(child_ptr->parents_);

    printGroup(id, "parents", parents_set, output);
}

// Hyödynnetään getSiblings-apufunktiota sisarusten etsimiseen, ja tulostetaan
// sisarukset, tai virheilmoitus, jos henkilöä ei ole.

void Familytree::printSiblings(const std::__cxx11::string &id,
                               std::ostream &output) const
{
    Person* sibling = getPointer(id);
    if (sibling == nullptr){
        output << "Error. " << id << " not found." << std::endl;
        return;
    }
    std::vector<Person*> siblings = getSiblings(sibling);

    IdSet sibling_set = VectorToIdSet(siblings);

    printGroup(id, "siblings", sibling_set, output);

}

// Haetaan halutun henkilön serkut etsimällä vanhempien sisarukset, ja lisää-
// mällä näiden lapset cousins-vectoriin. Jos henkilöä ei ole olemassa, tulos-
// tetaan virheilmoitus.

void Familytree::printCousins(const std::__cxx11::string &id,
                              std::ostream &output) const
{
    Person* person_ptr = getPointer(id);

    if (person_ptr == nullptr){
        output << "Error. " << id << " not found." << std::endl;
        return;
    }
    else if (person_ptr->parents_.empty()){
        output << "Error. " << id << " has no cousins." << std::endl;
        return;
    }
    std::vector<Person*> parents = person_ptr->parents_;
    std::vector<Person*> aunts_n_uncles = {};
    std::vector<Person*> cousins {};

    for (auto parent : parents){

        // Estetään ongelma tapauksessa, jossa ensimmäinen vanhmepi puuttuu
        // syystä tai toisesta, mutta toinen vanhempi on kirjattu
        if (parent == nullptr){
            continue;
        }

        aunts_n_uncles = getSiblings(parent);

        if (aunts_n_uncles.empty()){
            continue;
        }

        // Käydään läpi kaikki tädit ja sedät, ja lisätään näiden lapset
        // cousins-vectoriin.
        for (auto uncle_aunt : aunts_n_uncles){

            for (auto cousin : uncle_aunt->children_){
                cousins.push_back(cousin);
            }
        }
    }

    IdSet cousins_set = VectorToIdSet(cousins);

    printGroup(id, "cousins", cousins_set, output);

}

// Funktio tarkistaa, onko haettua henkilöä olemassa, ja tulostaa virheilmoituk-
// sen, jos henkiöä ei löydy. Jos henkilö on olemassa, haetaan sukuhaaran pisin
// henkilö heighRecursive-apufunktiolla, ja tulostetaan

void Familytree::printTallestInLineage(const std::__cxx11::string &id,
                                       std::ostream &output) const
{

    Person* person_ptr = getPointer(id);
    if (person_ptr == nullptr){
        output << "Error. " << id << " not found." << std::endl;
        return;
    }
    Person* tallest = heightRecursive(person_ptr, person_ptr, true);

    if (person_ptr == tallest){
        output << "With the height of " << person_ptr->height_ << ", "
               << person_ptr->id_ << " is the tallest person in his/her lineage."
               << std::endl;
        return;
    }

    output << "With the height of " << tallest->height_ << ", " << tallest->id_
           << " is the tallest person in " << person_ptr->id_ << "'s lineage."
           << std::endl;
}

// Toimii kuten printTallestInLineage, paitsi että heightRecursive-apufunktiolle
// annetaan parametriksi false.

void Familytree::printShortestInLineage(const std::__cxx11::string &id,
                                        std::ostream &output) const
{

    Person* person_ptr = getPointer(id);
    if (person_ptr == nullptr){
        output << "Error. " << id << " not found." << std::endl;
        return;
    }

    Person* tallest = heightRecursive(person_ptr, person_ptr, false);

    if (person_ptr == tallest){
        output << "With the height of " << person_ptr->height_ << ", "
               << person_ptr->id_ <<
                  " is the shortest person in his/her lineage." << std::endl;

        return;
    }
    output << "With the height of " << tallest->height_ << ", " <<
              tallest->id_ << " is the shortest person in " <<
              person_ptr->id_ << "'s lineage." << std::endl;

}

// Etsitään halutun henkilön lapsenlapset GrandRecursive-apufunktion avulla.
// Tulostetaan virheilmoitus, jos n on alle 1 tai jos henkillöä ei löydy.
// Boolean-parametrilla false saadaan GrandRecursive hakemaan lapsia.

void Familytree::printGrandChildrenN(const std::__cxx11::string &id,
                                     const int n, std::ostream &output) const
{

    if (n < 1){
        output << "Error. Level can't be less than 1." << std::endl;
        return;
    }

    Person *person_ptr = getPointer(id);

    if (person_ptr == nullptr){
        output << "Error. " << id << " not found." << std::endl;
        return;
    }
    IdSet tulos = grandRecursive(person_ptr, n, {}, false);

    printGroup(id, howGrand(n) + "grandchildren", tulos, output);

}

// Toimii kuten printGrandChildrenN, paitsi, että GrandRecursive-apufunktiolle
// annetaan parametriksi true, jolloin haetaan vanhempia.

void Familytree::printGrandParentsN(const std::__cxx11::string &id, const int n,
                                    std::ostream &output) const
{

    if (n < 1){
        output << "Error. Level can't be less than 1." << std::endl;
        return;
    }

    Person *person_ptr = getPointer(id);

    if (person_ptr == nullptr){
        output << "Error. " << id << " not found." << std::endl;
        return;
    }

    IdSet tulos = grandRecursive(person_ptr, n, {}, true);

    printGroup(id, howGrand(n) + "grandparents", tulos, output);

}

// ---------------------------- APUFUNKTIOT -------------------------------- //

// Palautetaan id:tä vastaava pointteri tai nullptr, jos id:tä ei löydy tieto-
// rakenteesta.

Person *Familytree::getPointer(const std::string &id) const{

    Personmap::const_iterator it = data_.find(id);

    if (it != data_.end()){
        return it->second.get();
    }
    return nullptr;
}

// Muutetaan pointterivector setiksi tulostusta varten. Näin tulokset saadaan
// aakkosjärjestykseen ja mahdolliset duplikaatit poistuvat.

IdSet Familytree::VectorToIdSet(const std::vector<Person*> &container) const
{
    IdSet ids;

    for (auto person: container){

        if (person == nullptr){
            continue;
        }
        ids.insert(person->id_);

    }
    return ids;
}

// Tallennetaan outputstreamiin henkilön halutut sukulaiset tai vaihtoehtoisesti
// ilmoitus siitä, ettei tiettyä sukulaistyyppiä ole.

void Familytree::printGroup(const std::__cxx11::string &id,
                            const std::__cxx11::string &group,
                            const IdSet &container, std::ostream &output) const{

    if (container.empty()){
        output << id << " has no " << group << "." << std::endl;
        return;
    }

    output << id << " has " << container.size() << " "
           << group << ":" << std::endl;

    for (auto name : container){

        output << name << std::endl;
    }
}

// Tulostetaan henkilön sukuhaaran pisin/lyhyin henkilö tai ilmoitus siitä, että
// henkilö on itse pisin.

void Familytree::printTallestOrShortest(Person* const &person,
                                        Person* const &tallest,
                                        const std::__cxx11::string tallshort,
                                        std::ostream &output) const {


    if (person->id_ == tallest->id_){
        output << "With the height of " << person->height_ << ", "
               << person->id_ << " is the" << tallshort
               << " person in his/her lineage." << std::endl;

        return;
    }
    output << "With the height of " << tallest->height_ << ", "
           << tallest->id_ << " is the " << tallshort << " person in "
           << person->id_ << "'s lineage." << std::endl;

}

// Apufunktio yhden henkilön sisarusten etsimiseen. Haetaan molempien vanhempien
// kaikki lapset, ja tallennetaan ne siblings-vectoriin. Henkillöä itseään ei
// luonnollisesti tallenneta. Duplikaattien poistaminen toteutuu luonnostaan
// VectorToIdSet-funktiossa.

std::vector<Person*> Familytree::getSiblings(Person * const &sibling_ptr) const
{
    std::vector<Person*> siblings = {};

    for (auto parent : sibling_ptr->parents_){

        if (parent == nullptr)
            continue;

        if (parent->children_.empty()){
            continue;
        }

        for (auto henkilo : parent->children_){
            if (henkilo->id_ != sibling_ptr->id_){
                siblings.push_back(henkilo);
            }
        }
    }

    return siblings;

}

// Palauttaa oikean määrän great-great-merkkijonoja isovanhempien ja lasten
// lasten tulostusta varten

std::__cxx11::string Familytree::howGrand(int n) const
{
    std::string grand = "";

    while (n > 1){
        --n;
        grand += "great-";
    }

    return grand;
}

// Haetaan halutun henkilön n. polven isovanhemmat tai lapsenlapset rekursiota
// käyttäen ja tallennetaan ne grands-settiin, joka palautetaan lopuksi kutsu-
// neelle funktiolle. porc (parent or child)-totuusarvo kertoo, etsitäänkö
// vanhempia vai lapsia. Molempien etsiminen onnistuu lähes samalla funktio-
// rakenteella.

IdSet Familytree::grandRecursive(Person* person_ptr, int n, IdSet grands,
                                 const bool &porc) const
{
    if (n == -1){
        grands.insert(person_ptr->id_);
        return grands;
    }
    if (porc){
        if (person_ptr->parents_.empty()){
            return grands;
        }
        for (auto parent : person_ptr->parents_){
            if (parent == nullptr){
                continue;
            }
            grands = grandRecursive(parent, n-1, grands, porc);
        }
    }
    else{
        if (person_ptr->children_.empty()){
            return grands;
        }
        for (auto child : person_ptr->children_){
            grands = grandRecursive(child, n-1, grands, porc);
        }
    }
    return grands;
}


// Etsitään yhdestä henkilöstä alkavan sukupuun pisin tai lyhin henkilö
// rekursiota käyttäen. Yhtä pitkien henkilöiden tapauksessa säilytetään
// aiemmin löytynyt. Totuusarvo tallest kertoo, haetaanko lyhyintä vai pisintä
// henkilöä. Molemmissa tapauksissa toiminnallisuus on hyvin pitkälti sama.
// Pisin tai lyhyin henkilö tallennetaan max_person-pointteriin ja palautetaan
// lopuksi kutsuneelle funktiolle.

Person *Familytree::heightRecursive (Person * const &person,
                                     Person *max_person,
                                     const bool &tallest) const {

    if (tallest){
        if (person->height_ > max_person->height_){
            max_person = person;
        }
    }
    else{
        if (person->height_ < max_person->height_){
            max_person = person;
        }
    }

    if (person->children_.empty()){
        return max_person;
    }

    for (auto child : person->children_){

        max_person = heightRecursive(child, max_person, tallest);
    }

    return max_person;

}
