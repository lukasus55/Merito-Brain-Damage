# MERITO BRAIN DAMAGE
`TODO: Opis gry` 

## 🚀 Getting Started: How to Set Up the Project

> **STOP! This project uses Git LFS (Large File Storage).**
>
> You **must** follow these steps *before* you clone the project, or you will only download small "pointer" files instead of the actual game assets (like models and textures).

### Why Do We Use LFS?

Git is great for tracking text (like code) but very bad at tracking large binary files (like `.uasset`, `.fbx`, or `.png` files). Unreal Engine projects are full of these.

Git LFS solves this by storing the large files on a separate server, keeping our repository fast and small.

### 1. One-Time Setup (Do This Once Per Computer)

1.  **Install Git LFS:** Go to the official [Git LFS website](https://git-lfs.github.com/) and download the installer.
2.  **Activate LFS:** Open a terminal (Command Prompt, PowerShell, or Git Bash) and run this command. You only ever have to do this once.
    ```bash
    git lfs install
    ```
    It will respond with `Git LFS initialized.`

### 2. Download (Clone) the Project

Now you are ready to clone the repository.

1.  Open your terminal and navigate to where you want to store the project.
2.  Run the `git clone` command:
    ```bash
    git clone https://github.com/LesyaLesnichok/Merito-Brain-Damage.git
    ```
3.  **Watch the clone output.** You will see the normal Git messages, followed by LFS messages.
    ```
    Downloading LFS objects: 100% (150/150), 1.2 GB | 25 MB/s, done.
    ```
    This may take a long time, as it's downloading all the large game assets.

### 3. You're Ready to Go!

That's it! You can now open the `.uproject` file in Unreal Engine and start working.

When you **push** and **pull** changes (either with the command line or GitHub Desktop), Git LFS will automatically handle the large files in the background.

---

### ⚠️ Troubleshooting

**"My maps won't load, or my textures are just pointers!"**

This means you cloned the project *before* running `git lfs install`, or the LFS download failed.

**To Fix It:**
1.  Open a terminal inside your project folder.
2.  Run this command to force-download all the large files:
    ```bash
    git lfs pull
    ```

## Authors
- [Elisey Avvakumov](https://github.com/LesyaLesnichok)
- [Łukasz Kostyk](https://github.com/lukasus55)
- [Wiktor Polak](https://github.com/Vektor1469)
- [Bartosz Wyrzykowski](https://github.com/BartoszMerito)
