# 3 užduotis "MuhVector" klasė

Eksperimentiniai std::vector klasės atitikmens MuhVector tyrimai.
<br /><br />

## Tirtų funkcijų pavyzdžiai:<br /><br />
### push_back()
Kodas:
```c++ 
int main ()
{
  MuhVector<int> myvector;
  int myint;

  std::cout << "Please enter some integers (enter 0 to end):\n"; //Ivedame 5 skaicius, iskaitant 0

  do {
    std::cin >> myint;
    myvector.push_back (myint);
  } while (myint);

  std::cout << "myvector stores " << int(myvector.size()) << " numbers.\n";
  std::cout << "Expected output: 5";
  return 0;
}
```
Rezultatas:
```c++
myvector stores 5 numbers.
Expected output: 5
```
<br /><br />
### pop_back()
Kodas:
```c++
int main ()
{
  MuhVector<int> myvector;
  int sum (0);
  myvector.push_back (100);
  myvector.push_back (200);
  myvector.push_back (300);

  while (!myvector.empty())
  {
    sum+=myvector.back();
    myvector.pop_back();
  }

  std::cout << "The elements of myvector add up to " << sum << '\n';
  std::cout << "Expected output 600";

  return 0;
}
```
Rezultatas:
```c++
The elements of myvector add up to 600
Expected output 600
```
<br /><br />
### at()
Kodas:
```c++
int main ()
{
  MuhVector<int> myvector (10);   // 10 zero-initialized ints

  // assign some values:
  for (unsigned i=0; i<myvector.size(); i++)
    myvector.at(i)=i;

  std::cout << "myvector contains:";
  for (unsigned i=0; i<myvector.size(); i++)
    std::cout << ' ' << myvector.at(i);
  std::cout << '\n';

  std::cout << "Expected output: 0 1 2 3 4 5 6 7 8 9";
  return 0;
}
```
Rezultatas:
```c++
myvector contains: 0 1 2 3 4 5 6 7 8 9
Expected output: 0 1 2 3 4 5 6 7 8 9
```
<br /><br />
### erase()
Kodas:
```c++
int main ()
{
  MuhVector<int> myvector;

  // set some values (from 1 to 10)
  for (int i=1; i<=10; i++) myvector.push_back(i);

  // erase the 6th element
  myvector.erase (myvector.begin()+5);

  // erase the first 3 elements:
  myvector.erase (myvector.begin(),myvector.begin()+3);

  std::cout << "myvector contains:";
  for (unsigned i=0; i<myvector.size(); ++i)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  std::cout << "Expected output: 4 5 7 8 9 10"; 

  return 0;
}
```
Rezultatas:
```c++
myvector contains: 4 5 7 8 9 10
Expected output: 4 5 7 8 9 10
```
<br /><br />
### shrink_to_fit()
Kodas:
```c++
int main ()
{
  MuhVector<int> myvector (100);
  std::cout << "1. capacity of myvector: " << myvector.capacity() << '\n';

  myvector.resize(10);
  std::cout << "2. capacity of myvector: " << myvector.capacity() << '\n';

  myvector.shrink_to_fit();
  std::cout << "3. capacity of myvector: " << myvector.capacity() << '\n' << '\n';

  std::cout << "1. Expected output: 100"<< '\n';
  std::cout << "2. Expected output: 100"<< '\n';
  std::cout << "3. Expected output: 10"<< '\n';

  return 0;
}
```
Rezultatas:
```c++
1. capacity of myvector: 100
2. capacity of myvector: 100
3. capacity of myvector: 10

1. Expected output: 100
2. Expected output: 100
3. Expected output: 10
```
<br /><br />
## Tuščio std::vector ir MuhVektor užpildymo analizė
- Pildyta buvo atitinkamu kiekiu int tipo kintamaisiais.
- 0 reškia, kad laikas buvo per trumpas kompiuteriui tiksliai išmatuoti.

|             | 10 000 | 100 000    | 1 000 000  | 10 000 000 | 100 000 000 |
|-------------|--------|------------|------------|------------|-------------|
| std::vector | 0 s    | 0,001001 s | 0,005998 s | 0,069858 s | 0,645952 s  |
| MuhVector   | 0 s    | 0,001002 s | 0,007019 s | 0,078728 s | 0,730173 s  |

**Išvados:**

- MuhVector sparta pastebimai mažesnė.
- Toliau didinant užpildomų skaičių kiekį, šis skirtumas taptų dar labiau pastebimas.
<br /><br />

### Atminties perskirstymų kiekis užpildant 100 000 000 elementų
- Tiek su std::vector, tiek su MuhVector perskirstymų kiekis buvo toks pat - 27 kartai.
<br /><br />

## 2-uzd v.2.0 versijos programos spartos analizė naudojant MuhVector konteinerį

- Naudotas buvo tas pats 100 000 studentų failas.
- Naudotas konteinerio atskyrimo metodas: sukuriamas vienas naujas konteineris, į kurį įrašomi atitinkami studentai, įrašytiejį į naują konteinerį pašalinami iš senojo.

| MuhVector                                 | 100 000  |
|-------------------------------------------|----------|
| Failo nuskaitymas                         | 0,1973 s |
| Studentų rūšiavimas                       | 0,0258 s |
| Studentų atskyrimas                       | 0,0027 s |
| Studentų išvedimas į du skirtingus failus | 0,0780 s |

| std::vector                               | 100 000  |
|-------------------------------------------|----------|
| Failo nuskaitymas                         | 0,1893 s |
| Studentų rūšiavimas                       | 0,0190 s |
| Studentų atskyrimas                       | 0,0093 s |
| Studentų išvedimas į du skirtingus failus | 0,0705 s |

**Išvados:**

- MuhVector sparta pastebimai mažesnė.
- Spartos skirtumai taptų dar ryškesni su didesniais studentų kiekiais.
- Naudojant šį atskyrimo metodą, MuhVector sparta didesnė už std::vector.
