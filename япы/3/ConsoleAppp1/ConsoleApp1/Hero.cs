using System;

namespace AutoBattler
{
    public abstract class Hero : IAtackable
    {
        public int HP { get; set; }
        public int Damage { get; set; }
        public int Defense { get; set; }
        public string Name { get; set; }

        public Hero(string name, int hp, int defense, int damage)
        {
            Name = name;
            Defense = defense;
            HP = hp;
            Damage = damage;
        }

        public void TakeDamage(int damage)
        {
            if (damage <= 0) return;
            int effective = Math.Max(0, damage - Defense);
            HP -= effective;
            if (HP < 0) HP = 0;
        }

        public override string ToString()
        {
            return $"[Name = {Name} HP = {HP} Defense = {Defense} Damage = {Damage}]";
        }

        // قابلیت ویژه را کلاس‌های فرزند بازنویسی می‌کنند
        public virtual void SpecialAbility(Hero target) { }
    }
}
