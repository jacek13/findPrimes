# Script for running primes.exe
$begin = 0
$end = 1000000
$dllType = "ASM", "C/C++"
$numberOfThreads = 1, 2, 4, 8, 16, 32, 64
$numberOfCalculations = 10
$intervalTypes = "SMALL", "MEDIUM", "LARGE"
$architectureType = "-x86"
$programLoop = 1
$threads = 1
do
{
    do
    {
    echo "Welcome to script for running primes.exe with 3 different intervals."
    echo "You can select small, medium or large interval."
    $selectionString = Read-Host -Prompt "To do that type one from 3 options above"
    }while( $intervalTypes -notcontains $selectionString.ToUpper())

    do
    {
    echo "Select number of threads."
    $threads = Read-Host -Prompt "You can type (1, 2, 4, 8, 16, 32 or 64)"
    }while( $numberOfThreads -notcontains $threads)

    switch($selectionString.ToUpper())
    {
        "SMALL"{$end = 1000000}
        "MEDIUM"{$end = 10000000}
        "LARGE"{$end = 20000000}
    }

    # This script is for small interval
    for($i=1; $i -le 3; $i++)
    {
        switch($i)
        {
            1 {$dllType = "C/C++"}
            2 {$dllType = "ASM"; $architectureType = "-x86"}
            3 {$architectureType = "-x64"}
        }
         & "..\primes\x64\Release\primes.exe" -begin $begin -end $end -dll $dllType -T $threads -N $numberOfCalculations $architectureType
    }

    $doAgain = Read-Host -Prompt "Do you want to close script? Type true to do it"
    if($doAgain.ToUpper() -eq "TRUE")
    {
        $programLoop = 0
    }
        
}while($programLoop -eq 1)