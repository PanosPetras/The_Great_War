#!/usr/bin/env python3
"""Generate map/States/StateResources.txt.

Run with no arguments to write the file, or with --report to print the country
shares and the biggest producers per good without touching it.

One line per state, eleven space-separated integers in the order the raw goods
appear in Stockpile.h:

    Coal Oil Timber Rubber Cotton Iron Copper Bauxite Nitrates Grain Fruit

Each column is scored independently and then normalised so that the world's
single biggest producer of that good in ~1913 sits at 100.  Point-source goods
(the mines and the oil fields) come only from the hotspot tables below; the
broadly farmed and logged goods get a climate-and-population baseline on top,
so a state with no named deposit still grows food if it is somewhere food grows.
"""

import os, sys, math

# Run from anywhere; the data sits next to this script's repository.
SD = os.path.join(os.path.dirname(os.path.abspath(__file__)), os.pardir, "map", "States")

GOODS = ["Coal","Oil","Timber","Rubber","Cotton","Iron",
         "Copper","Bauxite","Nitrates","Grain","Fruit"]

names = [l.rstrip("\n") for l in open(os.path.join(SD,"StateNames.txt"))]
owners = [l.rstrip("\n") for l in open(os.path.join(SD,"StateOwners.txt"))]
pops   = [int(l) for l in open(os.path.join(SD,"StatePopulations.txt"))]
coords = [l.rstrip("\n") for l in open(os.path.join(SD,"StateCoordinates.txt"))]

N = len(names)
assert len(owners)==N and len(pops)==N and len(coords)==N, (len(owners),len(pops),len(coords),N)

# The map is an equirectangular 5616x2160; these two constants were fitted from
# Berlin, Moscow, Singapore, Cape Town and Sydney and land within a degree.
def latlon(c):
    if c.strip() == "---":
        return None
    x, y = c.split("---")
    return ((int(y) - 790) / 16.8, (int(x) - 2802) / 15.6)

# ---------------------------------------------------------------------------
# Hotspots.  Values are in rough shares of real 1913 output for that good, so
# only the ratios inside a column matter - the column gets rescaled at the end.
# ---------------------------------------------------------------------------

COAL = {
    # United States, 517 Mt.  Pennsylvania alone was about a third of it.
    "Pittsburgh":175,"Scranton":90,"Wheeling":72,"Huntington":40,"Harrisburg":22,
    "Williamsport":14,"Carbondale":38,"Springfield":24,"Peoria":16,"Columbus":34,
    "Marietta":26,"Louisville":30,"Bowling Green":22,"Knoxville":20,"Chattanooga":14,
    "Tuscaloosa":30,"Montgomery":10,"Evansville":18,"Pueblo":16,
    "Casper":10,"Denver":8,"Salt Lake City":10,"Billings":6,"Bismarck":5,
    "Roanoke":18,"Staunton":10,"Fort Wayne":6,"Des Moines":8,"Joplin":6,
    # United Kingdom, 292 Mt
    "Cardiff":57,"Cardigan":10,"Newcastle On Tyne":56,"Carlisle":8,"Sheffield":30,
    "Leeds":24,"Manchester":25,"Liverpool":10,"Glasgow":30,"Stirling":8,"Edinburgh":6,
    "Dumfries":5,"Nottingham":22,"Birmingham":18,"Chester":8,"Shrewesbury":6,
    "Leicester":8,"Bristol":6,
    # Germany, 279 Mt including a great deal of lignite
    "Dortmund":114,"Dusseldorf":30,"Cologne":16,"Aachen":12,"Kattowitz":43,
    "Oppeln":12,"Saarbrucken":17,"Halle":26,"Leipzig":20,"Cottbus":16,"Chemnitz":8,
    "Dessau":10,"Magdeburg":8,"Siegen":4,
    # Austria-Hungary, 54 Mt
    "Troppau":20,"Tesin":12,"Plzen":8,"Aussig":10,"Karlsbad":6,"Pecs":5,
    "Resicabanya":5,"Krakow":9,"Bielsko":4,"Banska Bystrica":3,
    # Russia, 36 Mt
    "Yuzovka":18,"Luhansk":9,"Kramatorsk":5,"Kielce":8,"Kemerevo":3,"Ekaterinburg":4,
    "Perm":3,"Chelyabinsk":3,"Irkutsk":2,"Tblisi":2,
    # France 40 Mt and Belgium 23 Mt
    "Lille":22,"Arras":10,"St Etienne":8,"Cambrai":4,"Alencon":0,
    "Charleroi":10,"Liege":8,"Namur":4,"Tournai":3,
    # The rest of the world
    "Fukuoka":12,"Ishikara":6,"Hakodate":3,"Nagasaki":4,
    "Bardwan":8,"Kharswari":6,"Raigarh":2,"Nagpur":2,
    "Taiyuan":5,"Mukden":4,"Kaifeng":3,"Fenzhou":2,"Datong":2,"Pingyang":2,
    "Cape Breton Island":7,"Fort MacLeod":4,"Medicine Hat":3,"Vancouver Island":3,
    "Newcastle":9,"Brisbane":3,"Ballarat":2,"Tasmania":2,
    "Lydenburg":5,"Ladysmith":3,"Johannesburg":2,
    "Oviedo":4,"Leon":2,"Ciudad Real":1,
    "Kastamonu":3,
    "Trencin":0,"Zagreb":2,"Sarajevo":3,"Bor":1,
    "Maastricht":2,"Turin":1,"Sassari":1,
}

OIL = {
    # United States, about two thirds of the world's oil
    "Los Angeles":97,"Monterey":30,"Sacramento":8,"Mariposa":6,
    "Okmulgee":63,"Tahlequah":20,"Oklahoma City":18,
    "Carbondale":24,"Springfield":6,
    "Wheeling":12,"Huntington":6,"Pittsburgh":8,"Erie":4,"Williamsport":3,
    "Houston":15,"Dallas":5,"Lubbock":2,
    "Shreveport":12,"Baton Rouge":4,"Wichita":4,"Topeka":2,
    # Russia, nearly all of it Baku
    "Baku":62,"Siyazen":6,"Grozny":9,"Guryev":2,"Ashkabad":1,
    # Everyone else
    "Tampico":30,"Veracruz":6,"Mintatitlan":4,
    "Tirgoviste":14,"Bucharest":4,"Braila":2,
    "Palembang":6,"Jambi":3,"Tarakan":3,"Samarinda":3,"Medan":2,
    "Magwe":9,"Prome":2,
    "Stryi":8,"Stanislavov":3,"Przemysl":1,
    "Abadan":3,"Ahvaz":2,
    "Niigata":2,"Akita":1,
    "Hanover":1,"Brunswick":1,
    "Trinidad":2,"Maracaibo":1,"Sorong":1,
}

IRON = {
    # United States, 62 Mt, and the Mesabi range was most of it
    "Duluth":30,"Marquette":8,"Tuscaloosa":6,"Montgomery":3,"Scranton":2,
    "Sault Ste Marie":2,"Grand Junction":1,
    # Germany 29 Mt, France 22 Mt - the Lorraine minette straddled the border
    "Metz":20,"Verdun":14,"Nancy":6,"Brunswick":5,"Siegen":4,"Osnabruck":2,
    "Caen":3,"Alencon":2,"Rennes":1,"Luxembourg":7,
    # United Kingdom, 16 Mt
    "Newcastle On Tyne":6,"Hull":4,"Carlisle":3,"Lincoln":3,"Leicester":2,
    "Sheffield":1,"Glasgow":1,
    # Spain 10 Mt, Sweden 7 Mt
    "BIlbao":7,"Santander":3,"Lulea":5,"Falun":3,"Gavle":1,
    # Russia 9 Mt
    "Krivoy Rog":6,"Kerch":2,"Ekaterinburg":3,"Perm":1,"Tula":1,
    # Austria-Hungary 5 Mt and the Balkans
    "Judenburg":3,"Graz":1,"Resicabanya":2,"Sarajevo":1,"Bor":1,
    # The rest
    "Bone":3,"Constantine":1,"Leghorn":2,"Millertown":3,"Saint Johns":2,
    "Keunjahr":2,"Sambalpur":1,"Mukden":1,"Hanyang":2,
    "Santiago de Cuba":2,"Camaguey":1,"Adelaide":2,"Broken Hill":1,
    "Larisa":1,
}

COPPER = {
    # Butte was the single biggest copper camp on earth
    "Bozeman":150,"Missoula":20,"Great Falls":15,
    "Tucson":90,"Phoenix":40,"Las Cruces":25,"Santa Fe":10,
    "Salt Lake City":70,"Elko":30,"Carson City":10,"Moab":6,
    "Marquette":45,"Denver":8,"Alamosa":4,
    # Chile, on the eve of Chuquicamata
    "Calama":18,"Antofagasta":10,"Santiago":14,"Copiapo":6,"La Serena":4,
    # Japan, Spain, Mexico, Russia, Peru
    "Utsunomiya":25,"Matsuyama":18,"Mito":12,"Akita":6,
    "Huelva":40,"Seville":6,
    "Nogales":30,"Chihuahua":12,"Parral":8,"Loreto":5,
    "Ekaterinburg":18,"Perm":6,"Ganja":6,"Kapan":5,
    "Huanuco":20,"Huaraz":6,
    # Everyone else
    "Halle":25,"Sudbury":18,"Tasmania":12,"Gladstone":10,"Broken Hill":8,
    "Bunkeya":7,"Kazembe":3,"Kitwe":2,
    "Trondheim":6,"Falun":4,"Oruro":5,"Potosi":4,"Bor":6,"Judenburg":2,
    "Nizwa":1,
}

BAUXITE = {
    # France was well over half the world's bauxite, all of it in Provence
    "Toulon":150,"Frejus":90,"Digne":40,"Avignon":20,"Nice":10,
    # Arkansas, which is where the town of Bauxite gets its name
    "Little Rock":95,"Hot Springs":30,"Atlanta":15,"Valdosta":6,"Tuscaloosa":5,
    # Istria and the Dalmatian karst, then a very small industry
    "Pola":8,"Split":5,"Mostar":4,"Livno":3,"Gorizia":2,
    "Leghorn":3,"Grosetto":2,"Georgetown":2,
}

# Chilean caliche was very nearly the whole world supply until Haber-Bosch
NITRATES = {
    "Iquique":120,"Antofagasta":80,"Arica":25,"Calama":18,"Copiapo":10,
    # Guano, and the first two synthetic plants
    "Ica":6,"Lima":3,"Piura":2,
    "Skien":9,"Drammen":3,
    "Mannheim":6,
}

RUBBER = {
    # Wild rubber out of the Amazon, already being overtaken by the plantations
    "Manaus":40,"Tefe":16,"Borba":10,"Tabatinga":8,"Tarauaca":10,"Belem":12,
    "Santarem":8,"Oriximina":5,"Altamira":5,"Maraba":4,
    "Iquitos":10,"Yurimagues":5,"Puca Urco":4,"Riberalta":8,"Cobija":5,
    # British Malaya and Ceylon
    "Kuala Lumpur":40,"Ipoh":25,"Malacca":15,"Johor Bahru":18,"Penang":12,
    "Kuantan":8,"Alor Setar":6,"Kota Bahru":5,"Singapore":6,
    "Colombo":18,"Kandy":14,"Jaffna":3,
    # The Indies and the rest
    "Medan":16,"Palembang":10,"Jambi":6,"Bencoolen":4,"Padang":4,"Pontianak":3,
    "Batavia":6,"Bogor":5,"Surabaya":3,
    "Kisangani":8,"Basoko":6,"Mungbane":4,"Nyangwe":4,"Uvira":3,"Irebu":3,
    "Kribi":3,"Douala":3,"Bertoua":2,"Libreville":2,"Loango":2,
    "Grand Bassam":2,"Sassandra":2,"Monrovia":1,
    "Saigon":3,"Kampot":2,"Rangoon":2,"Bangkok":1,
}

COTTON = {
    # The American South grew about three fifths of the world's cotton
    "Dallas":90,"Austin":60,"San Antonio":40,"Lubbock":30,"Laredo":15,"Houston":20,
    "Vicksburg":70,"Jackson":55,"Memphis":45,
    "Montgomery":50,"Tuscaloosa":35,"Mobile":15,
    "Atlanta":50,"Valdosta":35,"Savannah":20,
    "Columbia":35,"Charleston":15,"Charlotte":30,"Raleigh":20,"Winston-Salem":15,
    "Little Rock":40,"Fayetteville":20,"Hot Springs":10,
    "Shreveport":25,"Baton Rouge":20,"New Orleans":10,
    "Oklahoma City":25,"Okmulgee":15,"Nashville":12,"Paducah":8,"Tallahassee":8,
    # India
    "Ahmedabad":30,"Baroda":20,"Surat":16,"Rajkot":12,"Bombay":14,"Patan":8,
    "Nagpur":22,"Amarati":18,"Hyderabad":14,"Gulbarga":10,"Bijapur":8,
    "Madras":8,"Kurnool":8,"Nellore":6,"Coimbatore":6,"Indore":10,"Jubulpore":6,
    # Egypt, the finest long-staple crop in the world
    "Tanta":22,"Mansura":20,"Ziqaziq":16,"Dumyat":10,"Giza":8,"Cairo":6,
    "Minya":6,"Asyut":6,"Bani Suwayf":5,"Fayyum":5,
    # China
    "Shanghai":18,"Suzhou":14,"Taicangzhou":10,"Tongzhou":8,
    "Hanyang":10,"Wuchang":8,"Jinan":8,"Xian":5,"Baoqing":4,
    # Russian Turkestan
    "Kokand":18,"Osh":10,"Tashkent":12,"Samarkand":8,"Bukhara":8,"Merv":6,
    "Khiva":5,"Shymkent":4,"Ganja":3,
    # The rest
    "Izmir":8,"Adana":10,"Aleppo":6,"Antep":4,"Urfa":3,
    "Recife":8,"Fortaleza":6,"Natal":4,"Salvador de Bahia":4,"Sorocaba":4,
    "Piura":6,"Trujillo":4,"Ica":3,
    "Torreon":8,"Culiacan":4,
    "Sokoto":4,"Kano":6,"Zaria":4,"Bida":2,
    "Kasempa":1,"Khartoum":3,"Wad Madani":4,
    "Peshawar":4,"Multan":8,"Lahore":6,"Sukkur":5,"Karachi":4,
}

# Named forests worth more than the climate baseline alone would give them
TIMBER = {
    # Pacific Northwest and the Great Lakes cut-over, the two big American booms
    "Seattle":60,"Portland":50,"Spokane":20,"Baker City":15,"Eureka":25,
    "Klamath Falls":18,"San Francisco":8,"Coeur D'Alene":16,"Missoula":12,
    "Duluth":30,"Eau Claire":25,"Green Bay":20,"Marquette":22,"Traverse City":18,
    "Saginaw":15,"Grand Rapids":10,"Minneapolis":12,"Moorehead":6,
    "Bangor":20,"Caribou":14,"Burlington":8,"Concord":8,"Lewiston":10,
    "Mobile":18,"Biloxi":16,"Shreveport":18,"Lake Charles":16,"Baton Rouge":12,
    "Jacksonville":14,"Savannah":12,"WIlmington":10,
    # Canada
    "Vancouver":40,"Vancouver Island":25,"Prince Rupert":15,"Prince George":12,
    "Thunder Bay":14,"Sudbury":10,"Ottawa":12,"Trois Rivieres":14,"Chicoutimi":12,
    "Sherbrooke":8,"Fredericton":10,"Bathurst":8,"Halifax":6,
    # Fennoscandia and Russia
    "Sundsvall":30,"Gavle":20,"Ostersund":16,"Umea":16,"Lulea":14,"Falun":10,
    "Karlstad":10,"Orebro":6,
    "Trondheim":14,"Bergen":8,"Christiania":10,"Skien":8,"Drammen":8,"Hamar":8,
    "Oulu":14,"Kajaani":12,"Kuopio":12,"Mikkeli":10,"Tampere":8,"Viipuri":10,
    "Archangel":26,"Vologda":18,"Kotlas":14,"Ust Sysolsk":12,"Petrozavorsk":10,
    "Perm":14,"Vyatkha":12,"Nizhni Novgorod":10,"Vetluga":10,"Kostorma":8,
    "Tomsk":8,"Krasnoyarsk":8,"Irkutsk":8,"Tobolsk":6,"Kirensk":5,"Lensk":4,
    # Central Europe
    "Innsbruck":8,"Salzburg":8,"Klagenfurt":6,"Graz":6,"Judenburg":6,
    "Karlsbad":8,"Budejovice":8,"Banska Bystrica":8,"Cluj":8,"Suceava":8,
    "Bregenz":4,"Lienz":4,"Beszterce":6,"Cernauti":6,
    "Munich":8,"Nuremburg":6,"Freiburg":8,"Tubingen":6,"Bayreuth":6,"Fulda":4,
    "Bern":4,"Chur":4,"Lucern":3,
    "Bacau":6,"Tirgu Jiu":4,
    # Tropical hardwood, a much smaller trade
    "Belem":6,"Manaus":6,"Curitiba":8,"Lajes":6,"Sao Paulo":4,
    "Kisangani":4,"Basoko":4,"Douala":4,"Kribi":3,"Grand Lahou":3,"Sekondi":4,
    "Rangoon":8,"Toungoo":6,"Moulmein":6,"Prome":4,"Bassein":4,
    "Banjarmasin":5,"Samarinda":5,"Pontianak":4,"Palembang":4,
    "Auckland":8,"Hokitika":6,"Nelson":4,
}

# Breadbaskets, on top of the population-and-climate baseline
GRAIN = {
    "Wichita":40,"Topeka":35,"Goodland":25,"Omaha":30,"North Platte":22,
    "Alliance":14,"Des Moines":35,"Oskaloosa":25,"Cedar Rapids":25,"Sioux City":20,
    "Springfield":30,"Peoria":28,"Champaign":25,"Rockford":18,"Chicago":12,
    "Minneapolis":25,"Granite Falls":22,"Moorehead":18,"Bismarck":25,"Minot":22,
    "Dickinson":16,"Sioux Falls":22,"Dupree":12,"Kansas City":22,"Jefferson City":16,
    "Indianapolis":20,"Fort Wayne":16,"Columbus":16,"Dayton":14,"Madison":16,
    "Walla Walla":16,"Spokane":14,"Salt Lake City":6,
    # The Ukrainian and Volga black earth
    "Kiev":30,"Poltava":32,"Kharkov":30,"Cherson":28,"Odessa":26,"Ekaterinoslav":26,
    "Nikolaev":20,"Krivoy Rog":16,"Melitopol":18,"Simferopol":12,"Cherkassy":18,
    "Vinnitsa":18,"Zhitomir":14,"Chernigov":12,"Kursk":20,"Voronets":22,"Orel":16,
    "Tambov":20,"Penza":18,"Saratov":26,"Samara":28,"Syzran":18,"Simbirsk":16,
    "Ufa":18,"Orenburg":16,"Busuluk":12,"Novo Uzensk":12,"Tsaritsyn":16,
    "Ekaterinodar":26,"Stavropol":22,"Rostov":20,"Azov":14,"Chisinau":16,"Balti":12,
    "Omsk":16,"Tomsk":12,"Kokshetau":10,"Semipalatinsk":8,"Chelyabinsk":10,
    "Kazan":14,"Chistopol":12,"Warsaw":10,"Lublin":10,"Kalisz":8,
    # India and China, feeding the two largest populations on earth
    "Lahore":24,"Amritsar":20,"Multan":16,"Firuzapur":16,"Sialkot":12,"Shahpur":10,
    "Cawnpore":22,"Lucknow":22,"Allahabad":20,"Agra":18,"Meerut":16,"Delhi":12,
    "Benares":18,"Patna":22,"Gaya":16,"Bhagalpur":14,"Fyzabad":14,"Gorakhpur":16,
    "Calcutta":16,"Bardwan":18,"Dacca":22,"Rajshahi":16,"Jessore":14,"Chittagong":10,
    "Madras":14,"Tanjore":16,"Madurai":12,"Cuttack":12,"Nagpur":10,"Rangoon":22,
    "Bassein":16,"Pegu":14,"Prome":10,
    "Changsha":22,"Wuchang":18,"Hanyang":14,"Nanchang":16,"Jiujiang":12,
    "Shanghai":16,"Suzhou":18,"Yangzhou":14,"Huaian":14,"Nanjing":14,"Hangzhou":16,
    "Jinan":18,"Caozhou":16,"Taian":14,"Kaifeng":18,"Henan":14,"Runing":12,
    "Xian":14,"Taiyuan":10,"Chengdu":22,"Chongqing":18,"Canton":16,"Nanning":10,
    "Mukden":14,"Jilin":10,"Qiqihar":8,
    # Argentina, Canada, Australia - the new export farms
    "Buenos Aires":30,"Rosario":28,"La Plata":20,"Junin":18,"Azul":16,
    "Bahia Blanca":16,"Cordoba de Argentina":18,"Parana":14,
    "Villa Nueva":12,"Rio Cuarto":14,"Mar del Plata":10,
    "Regina":26,"Saskatoon":22,"Prince Albert":14,"Winnipeg":24,"Brandon":18,
    "Dauphin":12,"Calgary":14,"Edmonton":12,"Medicine Hat":10,"Maple Creek":10,
    "Adelaide":16,"Port Augusta":8,"Melbourne":12,"Swan Hill":10,"Wagga Wagga":12,
    "Moree":10,"Perth":8,"Kojonup":6,"Geraldton":6,"Toowoomba":8,
    # Europe, the Danube and the Nile
    "Budapest":18,"Szeged":20,"Debrecen":18,"Kecskemet":14,"Bekescsaba":14,
    "Oradea":12,"Temesvar":14,"Novi Sad":12,"Gyor":10,"Miskolc":8,
    "Bucharest":20,"Braila":18,"Craiova":16,"Galati":14,"Constanta":12,"Iasi":14,
    "Bacau":10,"Calarasi":12,"Sofia":8,"Pleven":10,"Ruse":10,"Stara Zagora":8,
    "Paris":10,"Orleans":14,"Chartres":14,"Bourges":12,"Troyes":10,"Amiens":12,
    "Toulouse":12,"Bordeaux":10,"Dijon":8,"Le Mans":8,
    "Magdeburg":12,"Halle":10,"Breslau":12,"Posen":12,"Konigsburg":10,"Stettin":8,
    "Hanover":8,"Munich":8,"Torun":8,"Allenstein":6,
    "Valladolid":10,"Burgos":8,"Salamanca":8,"Badajoz":8,"Seville":8,"Cordoba":8,
    "Bologna":10,"Ferrara":10,"Padua":8,"Milan":8,"Foggia":10,"Bari":8,"Catania":8,
    "Cambridge":8,"Lincoln":8,"Ipswich":8,"Chelmsford":6,"Canterbury":6,
    "Konya":12,"Ankara":10,"Eskishehir":8,"Adana":8,"Aleppo":8,"Mosul":8,"Baghdad":8,
    "Fayyum":10,"Minya":10,"Asyut":10,"Bani Suwayf":8,"Suhaj":8,"Tanta":10,"Giza":8,
    "Seoul":12,"Taegu":10,"Pyongyang":8,"Edo":10,"Nagoya":10,"Osaka":8,"Niigata":10,
    "Sendai":8,"Kumamoto":8,
}

FRUIT = {
    # California and Florida
    "Los Angeles":40,"San Diego":18,"Sacramento":25,"San Francisco":16,"Monterey":18,
    "Mariposa":12,"Tampa":22,"Jacksonville":16,"Tallahassee":8,
    "Rochester":10,"Buffalo":8,"Traverse City":8,"Winston-Salem":6,"Roanoke":6,
    # The Mediterranean, which is where most of the world's fruit trade came from
    "Valencia":40,"Castellon":25,"Alicante":22,"Murcia":20,"Malaga":18,"Almeria":14,
    "Seville":16,"Cordoba":12,"Huelva":8,"Balearic Islands":10,"Tarragona":10,
    "Barcelona":10,"Cadiz":8,"Granada":10,"Jaen":10,
    "Messina":30,"Palermo":28,"Catania":25,"Naples":20,"Salerno":16,"Bari":16,
    "Reggio di Calabria":14,"Rome":10,"Florence":10,"Bologna":10,"Genoa":8,
    "Sassari":8,"Cagliari":8,"Foggia":10,"Verona":10,
    "Faro":14,"Lisbon":10,"Setubal":8,"Evora":6,"Oporto":8,"Madeira":6,"Azores":6,
    "Marseilles":16,"Nice":12,"Perpignan":12,"Montpellier":12,"Avignon":12,
    "Toulon":10,"Bordeaux":10,"Frejus":8,"Corsica":6,
    "Patras":22,"Corinth":16,"Kalamata":14,"Zante":10,"Nafplion":10,"Attica":8,
    "Corfu":8,"Cyclades":6,"Missolonghi":6,
    "Izmir":26,"Manisa":18,"Denizli":12,"Antalya":10,"Mugla":8,"Bursa":10,
    "Chios":8,"Myteline":8,"Iraklion":8,"Chania":6,"Beirut":10,"Jaffa":14,"Sidon":6,
    "Damascus":8,"Homs":6,"Antioch":6,
    "Algiers":14,"Bougie":8,"Oran":10,"Constantine":8,"Tunis":10,"Gafsa":6,
    "Casablanca":8,"Rabat":6,"Marrakesh":8,"Fez":8,"Agadir":6,
    "Alexandria":8,"Cairo":6,
    # Bananas and the tropics
    "Limon":25,"San Jose":10,"La Ceiba":22,"San Pedro Sula":14,"Puerto Barrios":20,
    "Jamaica":18,"Santiago de Cuba":14,"Havana":12,"Camaguey":10,"Jagua":8,
    "Santo Domingo":8,"Cap Haitien":8,"Port Au Prince":6,"Belize":6,
    "Bluefields":6,"Panama City":8,"David":6,"Barranquilla":10,"Cartagena de Indias":8,
    "Medellin":10,"Guayaquil":12,"Manizales":6,
    "Sao Paulo":10,"Rio de Janeiro":8,"Salvador de Bahia":8,"Recife":6,"Campinas":8,
    "Mendoza":12,"San Juan":8,"Tucuman":10,
    "Santiago":10,"Valparaiso":8,"Talca":6,
    "Cape Town":10,"Worcester":8,"Port Elizabeth":6,
    "Adelaide":8,"Melbourne":6,"Perth":6,"Tasmania":6,"Auckland":6,
    "Bombay":8,"Madras":6,"Calicut":6,"Cochin":6,"Colombo":6,
    "Canton":10,"Fuzhou":8,"Chaozhou":6,"Wenzhou":6,"Taibei":6,
    "Manila":8,"Batavia":6,"Surabaya":6,"Medan":4,
    "Ootomari":0,"Wakayama":10,"Shizuoka":10,"Matsuyama":8,"Kagoshima":6,
    "Baku":4,"Kutaisi":8,"Tblisi":6,"Erivan":6,"Samarkand":8,"Bukhara":6,"Tashkent":6,
    "Shiraz":6,"Isfahan":6,"Tehran":4,"Mashhad":4,
    "Chisinau":8,"Simferopol":10,"Odessa":6,"Ekaterinodar":6,
    "Split":8,"Dubrovnik":6,"Zadar":6,"Mostar":6,"Fiume":4,
    "Sofia":6,"Plovdiv":10,"Stara Zagora":8,"Skopje":6,"Salonika":8,"Bitola":6,
    "Belgrade":6,"Nis":6,"Kragujevac":6,"Uzice":4,"Zagreb":6,"Pecs":6,"Budapest":6,
}

HOT = {"Coal":COAL,"Oil":OIL,"Timber":TIMBER,"Rubber":RUBBER,"Cotton":COTTON,
       "Iron":IRON,"Copper":COPPER,"Bauxite":BAUXITE,"Nitrates":NITRATES,
       "Grain":GRAIN,"Fruit":FRUIT}

# Goods that only ever come from a named deposit
POINT_SOURCE = {"Coal","Oil","Iron","Copper","Bauxite","Nitrates","Rubber","Cotton"}

# How much of a country's land was actually farmed and logged for market in 1910.
# Colonial holdings that barely produced for export sit low on purpose.
AG = {
    "usa":1.00,"can":0.85,"eng":0.80,"fra":0.85,"ger":0.95,"kuk":0.85,"rus":0.75,
    "ita":0.90,"spa":0.80,"por":0.70,"nhl":0.90,"bel":0.90,"swi":0.80,"den":1.00,
    "swe":0.80,"nor":0.65,"gre":0.70,"rom":0.95,"bul":0.90,"ser":0.85,"mon":0.60,
    "ott":0.60,"per":0.50,"afg":0.40,"arb":0.25,"egy":0.90,"mor":0.55,"aby":0.45,
    "chi":0.85,"jap":0.90,"cor":0.70,"sia":0.75,"nep":0.60,"bhu":0.50,
    "usa_col":0.60,"ast":0.70,"nzl":0.85,"saf":0.65,"lib":0.40,
    "arg":0.95,"bra":0.70,"chl":0.65,"pru":0.55,"bol":0.45,"col":0.60,"ven":0.55,
    "ecu":0.60,"par":0.60,"urg":0.90,"mex":0.65,"cub":0.85,"dom":0.70,"hai":0.70,
    "gua":0.65,"hon":0.60,"nic":0.60,"cor":0.70,"els":0.70,"pan":0.55,"ku":0.70,
}
DEFAULT_AG = 0.6

def band(lat, lo, hi, peak_lo, peak_hi):
    """A trapezoid: zero outside [lo,hi], one across [peak_lo,peak_hi]."""
    a = abs(lat)
    if a <= lo or a >= hi:
        return 0.0
    if a < peak_lo:
        return (a - lo) / (peak_lo - lo)
    if a > peak_hi:
        return (hi - a) / (hi - peak_hi)
    return 1.0

def grain_clim(lat):
    a = abs(lat)
    if a > 68: return 0.0
    if a > 60: return 0.15
    if a >= 30: return 1.0          # the temperate cereal belt
    if a >= 12: return 0.75         # monsoon rice and millet
    return 0.55                     # equatorial rice

def timber_clim(lat):
    a = abs(lat)
    if a > 72: return 0.0
    if a >= 55: return 0.9          # boreal
    if a >= 38: return 0.7          # mixed temperate
    if a >= 20: return 0.18         # thin, mostly cleared
    return 0.30                     # tropical hardwood, little of it cut

def fruit_clim(lat):
    a = abs(lat)
    if a > 58: return 0.0
    if a >= 45: return 0.2
    if a >= 28: return 1.0          # the Mediterranean and Californian latitudes
    if a >= 8:  return 0.45
    return 0.55                     # bananas

CLIM = {"Grain":grain_clim,"Timber":timber_clim,"Fruit":fruit_clim}

# How loud the climate-and-population baseline is next to the named hotspots.
# Grain is grown nearly everywhere people live, so its baseline carries that
# column; fruit leans harder on the named Mediterranean and tropical districts.
BASE = {"Grain":14.0, "Timber":10.0, "Fruit":3.5}

# Desert, high plateau and tundra.  These have a population to weight by but
# almost no land worth farming or logging, which latitude alone will not tell
# you - Lhasa and Kansas City sit at much the same distance from the equator.
BARREN = {
    "Lhasa","Chamdo","Shigatse","Lhodrak","Changtang","Ngari","Kegudo","Makhai",
    "Balekungomi","Taklamakan","Khotan","Kashgar","Aksu","Kumul","Tulta","Ili",
    "Dalandzadgad","Sainshand","Altai","Khovd","Bulgan","Khatgal","Urga",
    "Bayan Tumen","Manzhouli","Ningxia","Gansu","Shangzhou",
    "Taoudenni","Tessalit","Timbuktu","Araouane","Bir Murghayn","Tishit","Walata",
    "Shingit","Chenachene","In Salah","In Guezzam","Tin Zawatene","Tamanrasset",
    "Timimoun","Bechar","Tindouf","Ilizi","Ouargla","Tuggurt","Laghwat","Naama",
    "Bilma","Madama","Agades","Arlit","Kufe","Zouar","Faya","Mourdiah",
    "Murzuk","Sabha","Ghat","Ghadamis","Waddan","Kufra","Jaghbub","Matan as Sarah",
    "Siwa","Bawiti","Mut","Baris","Matruh","Sidi Barrani",
    "Rutbah","Kaf","Rafha","Hail","Halaban","Sharawrah","Ruwayshid","Maan","Tabuk",
    "Nouakchott","Aleg","Maghama","Rosso","Dakhla","Aaiun","Tarfaya","Ifni",
    "Alice Springs","Oodnadatta","Tarcoola","Tennant Creek","Tanami","Wiluna",
    "Sandy Desert","Gibson's Desert","Dampier","Broome","Roebourne","Esperance",
    "Antofagasta","Iquique","Arica","Calama","Copiapo","Puerto Deseado",
    "Rio Galleagos","Esquel","Curaco","Telen","Vinchina","Calingasta","Cochinoca",
    "Chukchi","Kanin","Kola","Amderma","Indigirka","Kolyma","Srednekolymsk",
    "Cokurdah","Tiksi","Kular","Saskylah","Hatanga","Novyj Kajak","Olenek","Ajhal",
    "Khatyrka","Navarin","Majn","Manily","Omolon","Anuj","Anjuj","Ambarcik",
    "Palana","Korf","Ugolnoe","Gizhiga","Okhotsk","Berezov","Aksarka","Taz",
    "Naryn Mar","Mezen","Petsamo","Kostomushka","Nunavut","Baffin Island",
    "Ungava","Arviat","Churchill","Inuvik","Echo Bay","Reliance","Fort Resolution",
    "Port Harrison","Fort Chimo","Nitchequon","Dutch Harbor","Unalakleet",
    "Elko","Las Vegas","Loa","Moab","Flagstaff",
    "Zahedan","Jask","Chabahar","Bam","Lengeh","Kangan","Sirjan","Birjand",
    "Dashhowuz","Nukus","Khiva","Aqtau","Turgay","Aralsk","Qaratal","Aktogay",
    "Kyzyl","Yaksa","Bauntovskiy","Utchan","Ulusamudan",
}
BARREN_FACTOR = 0.2

# ---------------------------------------------------------------------------
# Score every state
# ---------------------------------------------------------------------------

missing = {}
for good, table in HOT.items():
    nameset = set(names)
    for k in table:
        if k not in nameset:
            missing.setdefault(good, []).append(k)

scores = {g: [0.0]*N for g in GOODS}

# A fifth of the states have no pin on the map.  Rather than guess one
# latitude for all of them, each borrows the median latitude of the states it
# shares a country with.
_bytag = {}
for i in range(N):
    ll = latlon(coords[i])
    if ll:
        _bytag.setdefault(owners[i], []).append(ll[0])
FALLBACK_LAT = {t: sorted(v)[len(v)//2] for t, v in _bytag.items()}

for i in range(N):
    ll = latlon(coords[i])
    lat = ll[0] if ll else FALLBACK_LAT.get(owners[i], 35.0)
    ag = AG.get(owners[i], DEFAULT_AG)
    if names[i] in BARREN:
        ag *= BARREN_FACTOR
    # Population stands in for how much land was cleared and worked; the square
    # root keeps a single huge state from swamping a whole column.
    popw = math.sqrt(pops[i] / 250000.0)

    for good in GOODS:
        v = float(HOT[good].get(names[i], 0.0))
        if good not in POINT_SOURCE:
            v += CLIM[good](lat) * ag * popw * BASE[good]
        scores[good][i] = v

# ---------------------------------------------------------------------------
# Rescale each column so the world's biggest producer of that good sits at 100
# ---------------------------------------------------------------------------

out = {g: [0]*N for g in GOODS}
for good in GOODS:
    top = max(scores[good])
    if top <= 0:
        continue
    k = 100.0 / top
    for i in range(N):
        v = scores[good][i] * k
        # Anything that rounds to nothing but is not nothing becomes a 1, so a
        # marginal producer still trades rather than vanishing.
        out[good][i] = 100 if v >= 99.5 else (max(1, int(round(v))) if v > 0.15 else 0)

if "--report" in sys.argv:
    for good in GOODS:
        col = out[good]
        nz = [v for v in col if v]
        print(f"\n=== {good}: {len(nz)} producing states, total {sum(col)} ===")
        order = sorted(range(N), key=lambda i: -col[i])[:12]
        print("  " + ", ".join(f"{names[i]}({owners[i]}) {col[i]}" for i in order))
        bycountry = {}
        for i in range(N):
            bycountry[owners[i]] = bycountry.get(owners[i],0) + col[i]
        tot = sum(bycountry.values()) or 1
        top = sorted(bycountry.items(), key=lambda kv:-kv[1])[:8]
        print("  shares: " + ", ".join(f"{t} {100*v/tot:.0f}%" for t,v in top))
    if missing:
        print("\n!!! hotspot names that match no state:")
        for g, ks in missing.items():
            print(f"  {g}: {ks}")
    sys.exit(0)

with open(os.path.join(SD,"StateResources.txt"), "w") as f:
    for i in range(N):
        f.write(" ".join(str(out[g][i]) for g in GOODS) + "\n")
print(f"wrote {N} lines x {len(GOODS)} goods")
