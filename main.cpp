#include "mainwindow.h"
#include <QApplication>
#include <QAction>
#include <iostream>
#include "geneticFootAlgorithm.h"
#include <functional>
#include <type_traits>
#include "biasedroulette.h"
#include "elitism.h"
#include "tournament.h"
#include "composition_helper.h"
#include <ctime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    std::cout<<"ALGO STARTED"<<std::endl;

    ////////////////////////////////////PLAYERS////////////////////////////////

    player kevin("kevin","vermeulen");
    player samuel("samuel","potencier");
    player anthony("anthony","bouchiga");
    player tanguy ("tanguy","conrate");
    player clement("clement","decoster");
    player lombardo("lucas","lombardo");
    player enzo("enzo","di Mateis");
    player gilme("lucas","gilme");
    player salim("salim","zarouri");
    player symon("symon","black");
    player antoine("antoine","allart");
    player martin ("martin","thireau");
    player baptiste("baptiste","salbot");
    player hadrien ("hadrien","flamant");
    player dechirot ("lucas","dechirot");
    player maxime ("maxime","lamouret");


    samuel.add_position(position::MC,7);
    samuel.add_position(position::MDC,2);
    samuel.add_position(position::MOC,8);
    samuel.add_position(position::BU,4);

    samuel.add_synergy(kevin,1.5);

    kevin.add_position(position::MDC,4);
    kevin.add_position(position::MC,8);
    kevin.add_position(position::MOC,6);
    kevin.add_position(position::BU,7);

    kevin.add_synergy(samuel,1.5);
    kevin.add_synergy(tanguy,1.5);

    clement.add_position(position::MDC,7);
    clement.add_position(position::MOC,1);
    clement.add_position(position::MC,3);
    clement.add_position(position::BU,8);
    clement.add_position(position::DC,8);



    anthony.add_position(position::MDC,7);
    anthony.add_position(position::MC,4);
    anthony.add_position(position::MOC,1);
    anthony.add_position(position::BU,9);
    anthony.add_position(position::DC,9);


    tanguy.add_position(position::MDC,1);
    tanguy.add_position(position::MC,3);
    tanguy.add_position(position::BU,2);
    tanguy.add_position(position::MOC,7);
    tanguy.add_position(position::DC,7);


    tanguy.add_synergy(kevin,1.5);

    lombardo.add_position(position::MDC,8);
    lombardo.add_position(position::MC,5);
    lombardo.add_position(position::MOC,5);
    lombardo.add_position(position::BU,5);
    lombardo.add_position(position::DC,5);



    enzo.add_position(position::MDC,5);
    enzo.add_position(position::MC,5);
    enzo.add_position(position::MOC,5);
    enzo.add_position(position::BU,5);
    enzo.add_position(position::DC,5);

    gilme.add_position(position::MDC,5);
    gilme.add_position(position::MC,5);
    gilme.add_position(position::MOC,5);
    gilme.add_position(position::BU,5);
    gilme.add_position(position::DC,4);
    gilme.add_position(position::DD,6);
    gilme.add_position(position::DG,1);



    antoine.add_position(position::MC,4);
    antoine.add_position(position::MDC,6);
    antoine.add_position(position::MOC,8);
    antoine.add_position(position::BU,2);
    antoine.add_position(position::DC,2);
    antoine.add_position(position::DG,2);
    antoine.add_position(position::DD,2);


    symon.add_position(position::MC,7);
    symon.add_position(position::DC,2);
    symon.add_position(position::DG,8);
    symon.add_position(position::BU,4);
    symon.add_position(position::AG,8);


    martin.add_position(position::AG,3);
    martin.add_position(position::DD,10);
    martin.add_position(position::DG,1);
    martin.add_position(position::BU,4);
    martin.add_position(position::DC,8);



    salim.add_position(position::BU,8);

    salim.add_position(position::AG,7);
    salim.add_position(position::MOC,6);

    baptiste.add_position(position::DD,1);
    baptiste.add_position(position::DC,5);
    baptiste.add_position(position::DG,6);
    baptiste.add_position(position::MDC,2);

    hadrien.add_position(position::DD,7);
    hadrien.add_position(position::DC,2);
    hadrien.add_position(position::MC,3);
    hadrien.add_position(position::MDC,9);
    hadrien.add_position(position::AG,10);


    dechirot.add_position(position::BU,4);
    dechirot.add_position(position::AG,10);
    dechirot.add_position(position::DC,1);
    dechirot.add_position(position::AD,4);


    maxime.add_position(position::AD,5);
    maxime.add_position(position::DC,1);
    maxime.add_position(position::MOC,2);
    maxime.add_position(position::DD,3);
    maxime.add_position(position::DG,4);
    maxime.add_position(position::BU,7);


    std::vector<player> v1 {kevin,samuel,tanguy,anthony,clement,lombardo,enzo,gilme,salim,antoine,maxime,dechirot,hadrien,baptiste,martin,symon};

    std::cout<<"TRY TO BALANCE "<< v1.size()<< " PLAYERS"<< std::endl;

    formation f(8);

    ////////////////////////////FORMATION//////////////////////////////////////////

    f.addPosition(position::MC);
    f.addPosition(position::MDC);
    f.addPosition(position::BU);
    f.addPosition(position::MOC);
    f.addPosition(position::DC);
    f.addPosition(position::DG);
    f.addPosition(position::DD);
    f.addPosition(position::AG);
    f.addSynergy(position::BU,position::MOC);
    f.addSynergy(position::MOC,position::MC);
    f.addSynergy(position::MC,position::MDC);

    //    std::vector<composition> possible = composition_helper::allPossibleComp(v1,f);


    //    for(auto it = possible.begin();it!=possible.end();it++)
    //    {
    //        std::cout<<it->calculGlobalValue()<<std::endl;
    //    }

    //    auto bestComp = composition_helper::getBestComp(v1,f);

    //    for(auto& comp:bestComp)
    //    {
    //        std::cout<<comp.getValue()<<std::endl;
    //    }

    formation f2(8);

    f2.addPosition(position::MC);
    f2.addPosition(position::MDC);
    f2.addPosition(position::BU);
    f2.addPosition(position::MOC);
    f2.addPosition(position::DC);
    f2.addPosition(position::DG);
    f2.addPosition(position::DD);
    f2.addPosition(position::AG);

    f2.addSynergy(position::BU,position::MOC);
    f2.addSynergy(position::MOC,position::MC);
    f2.addSynergy(position::MC,position::MDC);

    /////////////////ALGORITHM OPTIONS/////////////////////

    std::vector<std::unique_ptr<AlgorithmOption<std::pair<composition,composition>>>> options;

    BiasedRoulette<std::pair<composition,composition>> br(2);
    std::unique_ptr<BiasedRoulette<std::pair<composition,composition>>> ptrBiasedRoulette(&br);

    Elitism<std::pair<composition,composition>> el(2);
    std::unique_ptr<Elitism<std::pair<composition,composition>>> ptrElitism(&el);

    Tournament<std::pair<composition,composition>> to(2,0);
    std::unique_ptr<Tournament<std::pair<composition,composition>>> ptrTournament(&to);

    options.push_back(std::move(ptrBiasedRoulette));
    options.push_back(std::move(ptrElitism));
    options.push_back(std::move(ptrTournament));




    //////////////////////////////////PREREQUISITE FOR THE TEAMS////////////////////////////////////////////////

    std::vector<Predicate<const composition &, const composition &>> predicates;


    Predicate<const composition &,const composition &> pred1([&kevin,&tanguy](const composition & c,const composition &c2)mutable{
        if(!c.hasPlayer(kevin) && !c.hasPlayer(tanguy))
        {
            return false;
        }
        return true;
    });

    //auto possibilities = algo.getAllComb(v1,formats);

    //predicates.push_back(pred1);

    CompositionsAlgorithm algo(options);

    auto geneticDistrib = [&f,&f2,&v1,&predicates,&algo](std::vector<std::pair<composition,composition>> & currentGeneration){
        auto best = *std::max_element(currentGeneration.cbegin(),currentGeneration.cend(),composition_helper::comparePairs);
        currentGeneration.clear();
        currentGeneration.push_back(best);
        //auto newInitial = algo.generateInitialGeneration(v1,std::pair<formation,formation>(f,f2),99,predicates);
        auto newInitial = algo.generateClosestElements(best,99,predicates);
        currentGeneration.insert(currentGeneration.end(),newInitial.begin(),newInitial.end());
        return currentGeneration;
    };

    GeneticStrategy<std::pair<composition,composition>> stra(80,50,10,geneticDistrib);

    algo.setGeneticStrategy(stra);

    /////////////////////////////////////////////ASPECT TO EVALUATE ALGORITHM PERFORMANCE/////////////////////////////////////
    auto differentValuesLambda = [](auto & selected){
        int differentValues = 0;
        std::remove_reference_t<decltype(selected)>v;
        for(auto & possibility : selected)
        {
            if(std::find(v.cbegin(),v.cend(),possibility)==v.cend())
            {
                v.push_back(possibility);
                differentValues++;
            }
        }
        return differentValues;
    };

    auto timerFunc = [](auto && func, auto && ... params ){
        std::clock_t begin = clock();
        std::forward<decltype(func)>(func)(std::forward<decltype(params)> (params)...);
        std::clock_t end = clock();
        return double(end-begin)/CLOCKS_PER_SEC;
    };

    auto wrapperMemberFunc = [& algo](auto && ... params){
        algo.generateInitialGeneration(std::forward<decltype(params)>(params)...);
    };

    /////////////////////////////////////////////EXECUTION OF THE ALGORITHM///////////////////////////////////////

    //std::cout<<timerFunc(wrapperMemberFunc,v1,std::pair<formation,formation>(f,f2),150,predicates)<<std::endl;


    auto possibilities = algo.generateInitialGeneration(v1,std::pair<formation,formation>(f,f2),100,predicates);

    auto best = *std::max_element(possibilities.begin(),possibilities.end(),composition_helper::comparePairs);

    auto currentValue = composition_helper::calculValueSolution(best);
    auto currentStrength = composition_helper::calculStrengthSolution(best);

    std::cout<<" BEST ELEMENT VAL: " << currentValue << std::endl;
    std::cout<<" BEST ELEMENT  STR: " << currentStrength << std::endl;

    auto & selected = possibilities;

    std::cout<<"DIFFERENT VALUES AT THE BEGINNING : " << differentValuesLambda(selected) << std::endl;

    auto strategy = algo.getGeneticStrategy();

    for(int i=0;;i++)
    {
        static int sameBestGeneration = 0;

        double totalDeviation = 0;
        double strength=0;

        best = *std::max_element(selected.begin(),selected.end(),composition_helper::comparePairs);
        auto bestValue = composition_helper::calculValueSolution(best);
        auto bestStrength = composition_helper::calculStrengthSolution(best);
        std::cout<<" BEST ELEMENT VAL: "<< i<< " " << bestValue << std::endl;
        std::cout<<" BEST ELEMENT  STR: " << i << " " << bestStrength << std::endl;

        if(i!=0)
        {
            selected = algo.selection(selected,predicates);
            std::cout<<"DIFFERENT VALUES  : " << differentValuesLambda(selected) << std::endl;
        }

        selected = algo.crossover(selected,predicates);
        selected = algo.mutate(selected,f,f2,predicates);
        for(const auto & possibility : selected)
        {
            totalDeviation += std::abs(possibility.first.getValue()-possibility.second.getValue());
            strength+=possibility.first.getValue()+possibility.second.getValue();
        }


        if(sameBestGeneration==strategy.getNumberGenerationBeforeRedistribution())
        {
            std::cout<<"REDISTRIBUTING..."<<std::endl;
            strategy.redistribute(selected);
            sameBestGeneration = 0;
        }
        else if(currentValue == bestValue && currentStrength == bestStrength)
        {
            ++sameBestGeneration;
        }
        else
        {
            sameBestGeneration = 0;
        }
        currentValue = bestValue;
        currentStrength = bestStrength;

        std::cout << "TOTAL DEV : "<< totalDeviation <<std::endl;
        std::cout << "TOTAL STR : "<< strength <<std::endl;
        std::cout << "Number: "<< selected.size() <<std::endl;

    }


    //    for(int i=0;i<250;i++)
    //    {
    //        double totalDeviation = 0;
    //        double strength=0;
    //        selected = algo.selection(selected,predicates);
    //        for(const auto & possibility : selected)
    //        {
    //            totalDeviation += std::abs(possibility.first.getValue()-possibility.second.getValue());
    //            strength+=possibility.first.getValue()+possibility.second.getValue();
    //        }
    //        std::cout << "TOTAL DEV : "<< totalDeviation <<std::endl;
    //        std::cout << "TOTAL STR : "<< strength <<std::endl;
    //        std::cout << "Number: "<< selected.size() <<std::endl;

    //    }
    std::cout<<"END OF ALGO"<< std::endl;

    return a.exec();
}
