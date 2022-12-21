cd ..\lib

function Check-Repo ($repo_name) {

    $repo_link = "https://github.com/adafruit/"
    $full_repo_link = $repo_link + $repo_name

    if ( Test-Path -Path $repo_name )
    {
        Write-Output ("Pulling: " + $repo_name)
        git pull
        Write-Output ""
    }
    else
    {
        git clone $full_repo_link
        Write-Output ""
    }
}

Check-Repo("Adafruit-ST7735-Library")
Check-Repo("Adafruit-GFX-Library")
Check-Repo("Adafruit_BusIO")

# https://github.com/adafruit/Adafruit-ST7735-Library
# https://github.com/adafruit/Adafruit-GFX-Library
# https://github.com/adafruit/Adafruit_BusIO